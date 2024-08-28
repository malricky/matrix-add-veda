#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NOW() ({ struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts); ts; })
#define TIME(start, end) ((double)(end.tv_sec - start.tv_sec) * 1000.0 + (double)(end.tv_nsec - start.tv_nsec) / 1000000.0)

double* allocate_matrix(int rows, int cols) {
    return (double*)malloc(rows * cols * sizeof(double));
}

void fill_matrix_with_random_values(double* matrix, int rows, int cols) {
    for (int i = 0; i < rows * cols; i++) {
        matrix[i] = (double)rand() / RAND_MAX;
    }
}

void add_matrices(double* result, double* matrix1, double* matrix2, int rows, int cols) {
    struct timespec start, end;
	start = NOW();

    for (int i = 0; i < rows * cols; i++) {
        result[i] = matrix1[i] + matrix2[i];
    }

    end = NOW();
	double elapsed = TIME(start, end);
    printf("%d,%d,%lf\n",rows,cols,elapsed);
    //printf("\nMatrix Size: %d X %d\n",rows,cols);
    //printf("Total Elapsed Time: %f ms\n", elapsed);
}

void print_matrix(double* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%f ", matrix[i * cols + j]);
        }
        printf("\n");
    }
}

int main(int argc,char *argv[]) {
    int t = atoi(argv[1]);

        srand(time(NULL)); // Seed the random number generator

        int rows = t; // Number of rows
        int cols = t; // Number of columns

        // Allocate memory for matrices
        double* matrix1 = allocate_matrix(rows, cols);
        double* matrix2 = allocate_matrix(rows, cols);
        double* result = allocate_matrix(rows, cols);

        // Fill matrices with random values
        fill_matrix_with_random_values(matrix1, rows, cols);
        fill_matrix_with_random_values(matrix2, rows, cols);

        // Add the matrices
        add_matrices(result, matrix1, matrix2, rows, cols);

        // Print the matrices
        /*printf("Matrix 1:\n");
        print_matrix(matrix1, rows, cols);

        printf("\nMatrix 2:\n");
        print_matrix(matrix2, rows, cols);

        printf("\nResult (Matrix 1 + Matrix 2):\n");
        print_matrix(result, rows, cols);
        */

        // Free allocated memory
        free(matrix1);
        free(matrix2);
        free(result);

    return 0;
}
