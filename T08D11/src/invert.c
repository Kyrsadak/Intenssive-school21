#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int invert(double **matrix, int n, int m);
int input(double ***matrix, int *n, int *m);
void output(double **matrix, int n, int m);

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

void create_identity(double **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
}

int find_pivot_row(double **matrix, int col, int n) {
    int pivot_row = col;
    for (int i = col + 1; i < n; i++) {
        if (fabs(matrix[i][col]) > fabs(matrix[pivot_row][col])) {
            pivot_row = i;
        }
    }
    return pivot_row;
}

void swap_rows(double **matrix, double **inverse, int row1, int row2, int n) {
    for (int j = 0; j < n; j++) {
        double temp = matrix[row1][j];
        matrix[row1][j] = matrix[row2][j];
        matrix[row2][j] = temp;

        temp = inverse[row1][j];
        inverse[row1][j] = inverse[row2][j];
        inverse[row2][j] = temp;
    }
}

void scale_row(double **matrix, double **inverse, int row, double factor, int n) {
    for (int j = 0; j < n; j++) {
        matrix[row][j] *= factor;
        inverse[row][j] *= factor;
    }
}

void eliminate_row(double **matrix, double **inverse, int target_row, int pivot_row, double factor, int n) {
    for (int j = 0; j < n; j++) {
        matrix[target_row][j] -= factor * matrix[pivot_row][j];
        inverse[target_row][j] -= factor * inverse[pivot_row][j];
    }
}

int gauss_jordan_forward(double **matrix, double **inverse, int n) {
    for (int i = 0; i < n; i++) {
        int pivot_row = find_pivot_row(matrix, i, n);

        if (fabs(matrix[pivot_row][i]) < 1e-9) {
            return 0;
        }

        if (pivot_row != i) {
            swap_rows(matrix, inverse, i, pivot_row, n);
        }

        scale_row(matrix, inverse, i, 1.0 / matrix[i][i], n);

        for (int k = i + 1; k < n; k++) {
            if (fabs(matrix[k][i]) > 1e-9) {
                eliminate_row(matrix, inverse, k, i, matrix[k][i], n);
            }
        }
    }
    return 1;
}

void gauss_jordan_backward(double **matrix, double **inverse, int n) {
    for (int i = n - 1; i >= 0; i--) {
        for (int k = i - 1; k >= 0; k--) {
            if (fabs(matrix[k][i]) > 1e-9) {
                eliminate_row(matrix, inverse, k, i, matrix[k][i], n);
            }
        }
    }
}

int invert(double **matrix, int n, int m) {
    if (n != m) {
        return 0;
    }

    double **temp_matrix;
    double **inverse_matrix;

    if (!allocate_matrix(&temp_matrix, n, n)) {
        return 0;
    }
    if (!allocate_matrix(&inverse_matrix, n, n)) {
        free_matrix(temp_matrix, n);
        return 0;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            temp_matrix[i][j] = matrix[i][j];
        }
    }

    create_identity(inverse_matrix, n);

    if (!gauss_jordan_forward(temp_matrix, inverse_matrix, n)) {
        free_matrix(temp_matrix, n);
        free_matrix(inverse_matrix, n);
        return 0;
    }

    gauss_jordan_backward(temp_matrix, inverse_matrix, n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = inverse_matrix[i][j];
        }
    }

    free_matrix(temp_matrix, n);
    free_matrix(inverse_matrix, n);
    return 1;
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

void output(double **matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%.6f", matrix[i][j]);
            if (j < m - 1) {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void main() {
    double **matrix;
    int n, m;

    if (!input(&matrix, &n, &m)) {
        printf("n/a");
        return;
    }

    if (!invert(matrix, n, m)) {
        free_matrix(matrix, n);
        printf("n/a");
        return;
    }

    output(matrix, n, m);
    free_matrix(matrix, n);
}