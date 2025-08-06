#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH_LEN 256
#define MAX_TEXT_LEN 1024

typedef struct {
    char path[MAX_PATH_LEN];
    int loaded;
} Context;

void print_menu() {
    printf("1 - Enter path to file\n");
    printf("2 - Append text to loaded file\n");
    printf("-1 - Exit\n");
}

void read_and_print_file(const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        printf("n/a\n");
        return;
    }

    int empty = 1;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
        empty = 0;
    }
    fclose(file);

    if (empty) {
        printf("n/a\n");
    }
}

void load_file(Context *ctx) {
    if (scanf("%255s", ctx->path) != 1) {
        printf("n/a\n");
        return;
    }
    ctx->loaded = 1;
    read_and_print_file(ctx->path);
    printf("\n");
}

void append_to_file(const Context *ctx) {
    if (!ctx->loaded) {
        printf("n/a\n");
        return;
    }

    // Проверяем, существует ли файл
    FILE *check_file = fopen(ctx->path, "r");
    if (!check_file) {
        printf("n/a\n");
        return;
    }
    fclose(check_file);

    getchar(); // Очищаем буфер после scanf

    char buffer[MAX_TEXT_LEN];
    if (!fgets(buffer, MAX_TEXT_LEN, stdin)) {
        printf("n/a\n");
        return;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    FILE *file = fopen(ctx->path, "a");
    if (!file) {
        printf("n/a\n");
        return;
    }
    fprintf(file, "%s\n", buffer);
    fclose(file);

    read_and_print_file(ctx->path);
    printf("\n");
}

void handle_menu_choice(int choice, Context *ctx) {
    switch (choice) {
        case 1:
            load_file(ctx);
            break;
        case 2:
            append_to_file(ctx);
            break;
        default:
            printf("n/a\n");
            break;
    }
}



int main() {
    Context ctx = {"", 0};
    int choice;

    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("n/a\n");
            break;
        }
        if (choice == -1) {
            break;
        }
        handle_menu_choice(choice, &ctx);
    }

    return 0;
}