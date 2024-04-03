#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

int MATRIX_SIZE = 2048;
int VECTOR_SIZE = 2048;


void matvecmul_rowwise(double *matrix_block, double *vector, double *result_block, int block_size) {
    for (int i = 0; i < block_size; i++) {
        result_block[i] = 0.0;
        for (int j = 0; j < VECTOR_SIZE; j++) {
            result_block[i] += matrix_block[i * VECTOR_SIZE + j] * vector[j];
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

    int block_size = MATRIX_SIZE / size;

    double *matrix_block = (double *)malloc(block_size * VECTOR_SIZE * sizeof(double));
    double *vector_block = (double *)malloc(VECTOR_SIZE / size * sizeof(double));
    double *result_block = (double *)malloc(block_size * sizeof(double));
    double *vector = (double *)malloc(VECTOR_SIZE * sizeof(double));

    srand(rank);
    for (int i = 0; i < block_size * VECTOR_SIZE; i++) {
        matrix_block[i] = (double)rand() / RAND_MAX;
    }

    for (int i = 0; i < VECTOR_SIZE / size; i++) {
        vector_block[i] = (double)rand() / RAND_MAX;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    MPI_Allgather(vector_block, VECTOR_SIZE / size, MPI_DOUBLE, vector, VECTOR_SIZE / size, MPI_DOUBLE, MPI_COMM_WORLD);

    matvecmul_rowwise(matrix_block, vector, result_block, block_size);
    MPI_Barrier(MPI_COMM_WORLD);
    // double *result = (double *)malloc(MATRIX_SIZE * sizeof(double));
    // MPI_Gather(result_block, block_size, MPI_DOUBLE, result, block_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double end_time = MPI_Wtime();
        double elapsed_time = end_time - start_time;
        printf("Elapsed time: %f seconds\n", elapsed_time);
    }

    free(matrix_block);
    free(vector_block);
    free(result_block);
    // free(result);
    free(vector);

    MPI_Finalize();
    return 0;
}

