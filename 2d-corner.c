#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MATRIX_SIZE 2048
#define VECTOR_SIZE 2048

void matvecmul_2d_partition(double *matrix_block, double *vector_block, double *result_block, int block_rows, int block_cols) {
    for (int i = 0; i < block_rows; i++) {
        result_block[i] = 0.0;
        for (int j = 0; j < block_cols; j++) {
            result_block[i] += matrix_block[i * block_cols + j] * vector_block[j];
        }
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size % 2 != 0) {
        if (rank == 0) {
            printf("Error: Matrix size should be divisible by the number of processors, and the number of processors should be even.\n");
        }
        MPI_Finalize();
        return 1;
    }

    int block_cols = MATRIX_SIZE / 2; // Number of columns per processor
    int block_rows = MATRIX_SIZE * 2 / size; // Number of rows per processor

    // Allocate memory for matrix block
    double *matrix_block = (double *)malloc(block_rows * block_cols * sizeof(double));

    // Generate random matrix block for each processor
    srand(rank);
    for (int i = 0; i < block_rows * block_cols; i++) {
        matrix_block[i] = (double)rand() / RAND_MAX;
    }

    // Allocate memory for initial vector block
    double *init_vector_block = NULL;
    if (rank >= size / 2) {
        init_vector_block = (double *)malloc(VECTOR_SIZE * 2 * sizeof(double) / size);
        for (int i = 0; i < VECTOR_SIZE*2 / size; i++) {
            init_vector_block[i] = (double)rand() / RAND_MAX;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();


    // Determine whether the processor belongs to the left or right half
    int color = rank < size / 2 ? 0 : 1;

    // Create new communicator splitting into two halves
    MPI_Comm half_comm;
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &half_comm);
    
    double *complete_vector_block = (double *)malloc(VECTOR_SIZE * sizeof(double));
    if (rank >= size / 2) {
        // Perform all-to-all communication along the column dimension to distribute the complete vector
        MPI_Allgather(init_vector_block, VECTOR_SIZE * 2 / size, MPI_DOUBLE, complete_vector_block, VECTOR_SIZE * 2 / size, MPI_DOUBLE, half_comm);
    }
    
    // Send the first half of complete vector block from the right half processors to the corresponding left half processors
    if (rank < size / 2) {  
        MPI_Recv(complete_vector_block, VECTOR_SIZE / 2, MPI_DOUBLE, rank + size / 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        MPI_Send(complete_vector_block, VECTOR_SIZE / 2, MPI_DOUBLE, rank - size / 2, 0, MPI_COMM_WORLD);
    }
    
    
    // Allocate memory for result block
    double *result_block = (double *)malloc(block_rows * sizeof(double));
    
    // Compute matrix-vector product
    if (rank < size / 2) {
        matvecmul_2d_partition(matrix_block, complete_vector_block, result_block, block_rows, block_cols);
    } else {
        matvecmul_2d_partition(matrix_block, complete_vector_block + VECTOR_SIZE / 2, result_block, block_rows, block_cols);
    }
 
    // Perform reduction along row level
    double *row_results = (double *)malloc(block_rows * sizeof(double));
    if (rank < size / 2)
        MPI_Reduce(result_block, row_results, block_rows, MPI_DOUBLE, MPI_SUM, rank + size / 2, MPI_COMM_WORLD);
    else {
        MPI_Reduce(result_block, row_results, block_rows, MPI_DOUBLE, MPI_SUM, rank, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        double end_time = MPI_Wtime();
        double elapsed_time = end_time - start_time;
        printf("Elapsed time: %f seconds\n", elapsed_time);
    }

    // Free allocated memory
    free(matrix_block);
    if (init_vector_block != NULL) free(init_vector_block);
    free(complete_vector_block);
    free(result_block);
    free(row_results);

    MPI_Finalize();
    return 0;
}
