#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int sle(double **matrix, int n, int m, double *roots);
int input(double ***matrix, int *n, int *m);
void output(double **matrix, int n, int m);
void output_roots(double *roots, int n);

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

int find_pivot_row(double **matrix, int col, int n) {
    int pivot_row = col;
    for (int i = col + 1; i < n; i++) {
        if (fabs(matrix[i][col]) > fabs(matrix[pivot_row][col])) {
            pivot_row = i;
        }
    }
    return pivot_row;
}

void swap_rows(double **matrix, int row1, int row2, int m) {
    for (int j = 0; j < m; j++) {
        double temp = matrix[row1][j];
        matrix[row1][j] = matrix[row2][j];
        matrix[row2][j] = temp;
    }
}

void eliminate_column(double **matrix, int pivot_row, int n, int m) {
    for (int i = pivot_row + 1; i < n; i++) {
        if (fabs(matrix[i][pivot_row]) > 1e-9) {
            double factor = matrix[i][pivot_row] / matrix[pivot_row][pivot_row];
            for (int j = pivot_row; j < m; j++) {
                matrix[i][j] -= factor * matrix[pivot_row][j];
            }
        }
    }
}

int forward_elimination(double **matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        int pivot_row = find_pivot_row(matrix, i, n);

        if (fabs(matrix[pivot_row][i]) < 1e-9) {
            return 0;
        }

        if (pivot_row != i) {
            swap_rows(matrix, i, pivot_row, m);
        }

        eliminate_column(matrix, i, n, m);
    }
    return 1;
}

int back_substitution(double **matrix, int n, int m, double *roots) {
    for (int i = n - 1; i >= 0; i--) {
        if (fabs(matrix[i][i]) < 1e-9) {
            return 0;
        }

        roots[i] = matrix[i][n];

        for (int j = i + 1; j < n; j++) {
            roots[i] -= matrix[i][j] * roots[j];
        }

        roots[i] /= matrix[i][i];
    }
    return 1;
}

int check_consistency(double **matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        int all_zero = 1;
        for (int j = 0; j < n; j++) {
            if (fabs(matrix[i][j]) > 1e-9) {
                all_zero = 0;
                break;
            }
        }
        if (all_zero && fabs(matrix[i][n]) > 1e-9) {
            return 0;
        }
    }
    return 1;
}

int sle(double **matrix, int n, int m, double *roots) {
    if (m != n + 1) {
        return 0;
    }

    double **temp_matrix;
    if (!allocate_matrix(&temp_matrix, n, m)) {
        return 0;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            temp_matrix[i][j] = matrix[i][j];
        }
    }

    if (!forward_elimination(temp_matrix, n, m)) {
        free_matrix(temp_matrix, n);
        return 0;
    }

    if (!check_consistency(temp_matrix, n, m)) {
        free_matrix(temp_matrix, n);
        return 0;
    }

    if (!back_substitution(temp_matrix, n, m, roots)) {
        free_matrix(temp_matrix, n);
        return 0;
    }

    free_matrix(temp_matrix, n);
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
        if (i < n - 1) {
            printf("\n");
        }
    }
}

void output_roots(double *roots, int n) {
    for (int i = 0; i < n; i++) {
        printf("%.6f", roots[i]);
        if (i < n - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

void main() {
    double **matrix;
    double *roots;
    int n, m;

    if (!input(&matrix, &n, &m)) {
        printf("n/a");
        return;
    }

    roots = (double *)malloc(n * sizeof(double));
    if (roots == NULL) {
        free_matrix(matrix, n);
        printf("n/a");
        return;
    }

    if (!sle(matrix, n, m, roots)) {
        free_matrix(matrix, n);
        free(roots);
        printf("n/a");
        return;
    }

    output_roots(roots, n);
    free_matrix(matrix, n);
    free(roots);
}