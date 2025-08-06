#include <stdio.h>
#include <stdlib.h>

void sort_vertical(int **matrix, int n, int m, int **result_matrix);
void sort_horizontal(int **matrix, int n, int m, int **result_matrix);
int input(int ***matrix, int *n, int *m);
void output(int **matrix, int n, int m);

int compare(const void *a, const void *b) { return (*(int *)a - *(int *)b); }

int allocate_matrix(int ***matrix, int n, int m) {
    *matrix = (int **)malloc(n * sizeof(int *));
    if (*matrix == NULL) {
        return 0;
    }

    for (int i = 0; i < n; i++) {
        (*matrix)[i] = (int *)malloc(m * sizeof(int));
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

void free_matrix(int **matrix, int n) {
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void sort_vertical(int **matrix, int n, int m, int **result_matrix) {
    int *elements = (int *)malloc(n * m * sizeof(int));
    int idx = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            elements[idx] = matrix[i][j];
            idx++;
        }
    }

    qsort(elements, n * m, sizeof(int), compare);

    idx = 0;
    for (int j = 0; j < m; j++) {
        if (j % 2 == 0) {
            for (int i = 0; i < n; i++) {
                result_matrix[i][j] = elements[idx];
                idx++;
            }
        } else {
            for (int i = n - 1; i >= 0; i--) {
                result_matrix[i][j] = elements[idx];
                idx++;
            }
        }
    }

    free(elements);
}

void sort_horizontal(int **matrix, int n, int m, int **result_matrix) {
    int *elements = (int *)malloc(n * m * sizeof(int));
    int idx = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            elements[idx] = matrix[i][j];
            idx++;
        }
    }

    qsort(elements, n * m, sizeof(int), compare);

    idx = 0;
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            for (int j = 0; j < m; j++) {
                result_matrix[i][j] = elements[idx];
                idx++;
            }
        } else {
            for (int j = m - 1; j >= 0; j--) {
                result_matrix[i][j] = elements[idx];
                idx++;
            }
        }
    }

    free(elements);
}

int input(int ***matrix, int *n, int *m) {
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
            if (scanf("%d", &(*matrix)[i][j]) != 1) {
                free_matrix(*matrix, *n);
                return 0;
            }
        }
    }

    return 1;
}

void output(int **matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%d", matrix[i][j]);
            if (j < m - 1) {
                printf(" ");
            }
        }
        if (i < n - 1) {
            printf("\n");
        }
    }
}

int main(void) {
    int **matrix, **result;
    int n, m;

    if (!input(&matrix, &n, &m)) {
        printf("n/a");
        return 0;
    }

    if (!allocate_matrix(&result, n, m)) {
        free_matrix(matrix, n);
        printf("n/a");
        return 0;
    }

    sort_vertical(matrix, n, m, result);
    output(result, n, m);

    printf("\n\n");

    sort_horizontal(matrix, n, m, result);
    output(result, n, m);

    printf("\n");

    free_matrix(matrix, n);
    free_matrix(result, n);
    return 0;
}
