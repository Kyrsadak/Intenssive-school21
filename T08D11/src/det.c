#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double det(double **matrix, int n, int m);
int input(double ***matrix, int *n, int *m);
void output(double det);

int allocate_matrix(double ***matrix, int n, int m) {
    *matrix = (double **)malloc(n * sizeof(double *));
    if (*matrix == NULL) {
        return 0;
    }

    for (int i = 0; i < n; i++) {
        (*matrix)[i] = (double *)malloc(m * sizeof(double));
        if ((*matrix)[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free((*matrix)[j]);
            }
            free(*matrix);
            return 0;
        }
    }
    return 1;
}

void free_matrix(double **matrix, int n) {
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void copy_matrix(double **src, double **dest, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

void swap_rows(double **matrix, int row1, int row2, int n) {
    for (int j = 0; j < n; j++) {
        double temp = matrix[row1][j];
        matrix[row1][j] = matrix[row2][j];
        matrix[row2][j] = temp;
    }
}

int find_pivot(double **matrix, int col, int n) {
    int pivot_row = col;
    for (int k = col + 1; k < n; k++) {
        if (fabs(matrix[k][col]) > fabs(matrix[pivot_row][col])) {
            pivot_row = k;
        }
    }
    return pivot_row;
}

void eliminate_column(double **matrix, int col, int n) {
    for (int k = col + 1; k < n; k++) {
        double factor = matrix[k][col] / matrix[col][col];
        for (int j = col; j < n; j++) {
            matrix[k][j] -= factor * matrix[col][j];
        }
    }
}

double det(double **matrix, int n, int m) {
    if (n != m) {
        return 0.0;
    }

    double **temp;
    if (!allocate_matrix(&temp, n, n)) {
        return 0.0;
    }

    copy_matrix(matrix, temp, n);

    double determinant = 1.0;
    int sign = 1;

    for (int i = 0; i < n; i++) {
        int pivot_row = find_pivot(temp, i, n);

        if (fabs(temp[pivot_row][i]) < 1e-9) {
            free_matrix(temp, n);
            return 0.0;
        }

        if (pivot_row != i) {
            swap_rows(temp, i, pivot_row, n);
            sign = -sign;
        }

        determinant *= temp[i][i];
        eliminate_column(temp, i, n);
    }

    free_matrix(temp, n);
    return sign * determinant;
}

int input(double ***matrix, int *n, int *m) {
    if (scanf("%d %d", n, m) != 2) {
        return 0;
    }
    if (*n <= 0 || *m <= 0) {
        return 0;
    }

    if (!allocate_matrix(matrix, *n, *m)) {
        return 0;
    }

    for (int i = 0; i < *n; i++) {
        for (int j = 0; j < *m; j++) {
            if (scanf("%lf", &(*matrix)[i][j]) != 1) {
                free_matrix(*matrix, *n);
                return 0;
            }
        }
    }

    return 1;
}

void output(double det) { printf("%.6f\n", det); }

int main(void) {
    double **matrix;
    int n, m;
    double determinant;

    if (!input(&matrix, &n, &m)) {
        printf("n/a\n");
        return 0;
    }

    if (n != m) {
        free_matrix(matrix, n);
        printf("n/a\n");
        return 0;
    }

    determinant = det(matrix, n, m);
    output(determinant);
    free_matrix(matrix, n);
    return 0;
}
