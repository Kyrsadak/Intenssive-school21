#include "modules.h"

int get_last_id(FILE* ptr) {
    int id = -1;
    if (ptr != NULL) {
        fseek(ptr, -sizeof(modules), SEEK_END);
        fread(&id, sizeof(int), 1, ptr);
        fseek(ptr, 0, SEEK_SET);
    }
    return id;
}

int check_modules_id(FILE* ptr, int id) {
    int res = 0;
    if ((id >= 0) && (id <= get_last_id(ptr))) res = 1;
    return res;
}

int print_modules(FILE* ptr) {
    int is_error = 0;
    modules rec;
    if (ptr == NULL) {
        is_error = 1;
    } else {
        fseek(ptr, 0, SEEK_SET);
        fread(&rec, sizeof(modules), 1, ptr);
        while (!feof(ptr)) {
            printf("%d %s %d %d %d\n", rec.id, rec.name, rec.level, rec.cell, rec.flag);
            fread(&rec, sizeof(modules), 1, ptr);
        }
    }
    return is_error;
}

void get_name(char* name) {
    int size = 0;
    char c = ' ';
    scanf("%c", &c);
    if (c == '\n') scanf("%c", &c);
    while ((c != '\n') && (size != 30)) {
        name[size++] = c;
        scanf("%c", &c);
    }
    if (size > 30)
        printf("n/a\n");
    else
        name[size] = '\0';
}

modules get_modules_record(int id) {
    printf("Enter record\n");
    modules rec;
    rec.id = id;
    printf("name:\n");
    get_name(rec.name);
    printf("level:\n");
    while (1) {
        if (scanf("%d", &(rec.level)) != 1)
            printf("n/a\n");
        else
            break;
    }
    printf("cell:\n");
    while (1) {
        if (scanf("%d", &(rec.cell)) != 1)
            printf("n/a\n");
        else
            break;
    }
    printf("flag:\n");
    while (1) {
        if (scanf("%d", &(rec.flag)) != 1)
            printf("n/a\n");
        else
            break;
    }
    return rec;
}

int add_modules_record(FILE* ptr, modules rec) {
    int is_error = 0;
    if (ptr == NULL) {
        is_error = 1;
    } else {
        fseek(ptr, 0, SEEK_END);
        fwrite(&rec, sizeof(modules), 1, ptr);
        fseek(ptr, 0, SEEK_SET);
    }
    return is_error;
}

int delete_modules_record(FILE* ptr, int id) {
    int is_error = 0;
    if (ptr == NULL) {
        is_error = 1;
    } else {
        modules rec;
        int i = id;
        fseek(ptr, (i + 1) * sizeof(modules), SEEK_SET);
        while (!feof(ptr)) {
            fseek(ptr, (i + 1) * sizeof(modules), SEEK_SET);
            if (fread(&rec, sizeof(modules), 1, ptr) != 1) break;
            fseek(ptr, i * sizeof(modules), SEEK_SET);
            rec.id--;
            fwrite(&rec, sizeof(modules), 1, ptr);
            i++;
        }
        fseek(ptr, 0, SEEK_SET);
        truncate(modules_fpath, i * sizeof(modules));
    }
    return is_error;
}

int change_modules_record(FILE* ptr, int id, modules rec) {
    int is_error = 0;
    if (ptr == NULL) {
        is_error = 1;
    } else {
        fseek(ptr, id * sizeof(modules), SEEK_SET);
        fwrite(&rec, sizeof(modules), 1, ptr);
        fseek(ptr, 0, SEEK_SET);
    }
    return is_error;
}

int insert_modules_record(FILE* ptr, int id, modules new_rec) {
    int is_error = 0;
    if (ptr == NULL) {
        is_error = 1;
    } else {
        modules rec;
        int i;
        fseek(ptr, -sizeof(modules), SEEK_END);
        fread(&i, sizeof(int), 1, ptr);
        while (i >= id) {
            fseek(ptr, i * sizeof(modules), SEEK_SET);
            fread(&rec, sizeof(modules), 1, ptr);
            fseek(ptr, (i + 1) * sizeof(modules), SEEK_SET);
            rec.id++;
            fwrite(&rec, sizeof(modules), 1, ptr);
            i--;
        }
        fseek(ptr, (i + 1) * sizeof(modules), SEEK_SET);
        fwrite(&new_rec, sizeof(modules), 1, ptr);
        fseek(ptr, 0, SEEK_SET);
    }
    return is_error;
}

int select_modules(FILE* ptr, int count) {
    int is_error = 0;
    modules rec;
    int done = 0;
    int i = 0;
    if (count == 0) {
        print_modules(ptr);
        done = 1;
    }
    if (ptr == NULL) {
        is_error = 1;
    } else if (!done) {
        fseek(ptr, 0, SEEK_SET);
        fread(&rec, sizeof(modules), 1, ptr);
        while (!feof(ptr)) {
            if (i < count) {
                printf("%d %s %d %d %d\n", rec.id, rec.name, rec.level, rec.cell, rec.flag);
                fread(&rec, sizeof(modules), 1, ptr);
                i++;
            } else {
                break;
            }
        }
    }
    return is_error;
}
