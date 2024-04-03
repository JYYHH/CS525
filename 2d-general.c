#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <string.h>

int MATRIX_SIZE = 2048;
int VECTOR_SIZE = 2048;

void matvecmul_2d_partition(double *matrix_block, double *vector_block, double *result_block, int block_size) {
    for (int i = 0; i < block_size; i++) {
        result_block[i] = 0.0;
        for (int j = 0; j < block_size; j++) {
            result_block[i] += matrix_block[i * block_size + j] * vector_block[j];
        }
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Parse command line arguments
    if (argc == 3 && strcmp(argv[1], "-w") == 0) {
        MATRIX_SIZE = VECTOR_SIZE = atoi(argv[2]);
    } else if (argc != 1) {
        if (rank == 0) {
            printf("Usage: %s [-w matrix_size]\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    int sqrt_p = sqrt(size);
    int block_size = MATRIX_SIZE / sqrt_p; // Number of rows/columns per processor

    int dims[2], periods[2], coords[2];
    MPI_Comm grid_comm;
    dims[0] = dims[1] = sqrt_p;
    periods[0] = periods[1] = 0;
    MPI_Dims_create(size, 2, dims);
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &grid_comm);
    MPI_Cart_coords(grid_comm, rank, 2, coords);
    int my_row = coords[0];
    int my_col = coords[1];

    MPI_Comm col_comm;
    MPI_Comm_split(grid_comm, my_col, my_row, &col_comm);

    MPI_Comm row_comm;
    MPI_Comm_split(grid_comm, my_row, my_col, &row_comm);


    // Allocate memory for matrix block
    double *matrix_block = (double *)malloc(block_size * block_size * sizeof(double));

    // Generate random matrix block for each processor
    srand(rank);
    for (int i = 0; i < block_size * block_size; i++) {
        matrix_block[i] = (double)rand() / RAND_MAX;
    }

    // Allocate memory for vector block
    double *vector_block = (double *)malloc(block_size * sizeof(double));
    for (int i = 0; i < block_size; i++) {
        vector_block[i] = (double)rand() / RAND_MAX;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    // Perform one-to-one communication to exchange the last column's vector block to the main diagonal
    if (rank % sqrt_p == sqrt_p - 1) {
        int dest = rank - (sqrt_p - 1 - rank / sqrt_p);
        MPI_Send(vector_block, block_size, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
    }
    // If it is diagonal rank
    if (rank % sqrt_p == rank / sqrt_p) {
        int source = (1 + rank / sqrt_p) * sqrt_p - 1;
        MPI_Status status;
        MPI_Recv(vector_block, block_size, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, &status);
    }

    // Perform one-to-all communication along the column dimension
    MPI_Bcast(vector_block, block_size, MPI_DOUBLE, rank / sqrt_p, col_comm);
    
    // Allocate memory for result block
    double *result_block = (double *)malloc(block_size * sizeof(double));
    
    // Compute matrix-vector product
    matvecmul_2d_partition(matrix_block, vector_block, result_block, block_size);

    // Perform reduction along row level
    double *row_results = (double *)malloc(block_size * sizeof(double));
    MPI_Reduce(result_block, row_results, block_size, MPI_DOUBLE, MPI_SUM, 0, row_comm);

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        double end_time = MPI_Wtime();
        double elapsed_time = end_time - start_time;
        printf("Elapsed time: %f seconds\n", elapsed_time);
    }

    // Free allocated memory
    free(matrix_block);
    free(vector_block);
    free(result_block);
    free(row_results);

    MPI_Finalize();
    return 0;
}
