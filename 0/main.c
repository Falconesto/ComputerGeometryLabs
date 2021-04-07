#include <locale.h>
#include "stdlib.h"
#include "stdio.h"

enum Action {
    Inverse,
    HorizontalReflection,
    VerticalReflection,
    RightTurn,
    LeftTurn,
};

struct Config{
    char *input_file_name;
    char *output_file_name;
    enum Action action;
};

struct pgm_file {
    unsigned int format;
    unsigned int width;
    unsigned int height;
    unsigned int colours;
    unsigned int *data;
};


int main(int argc, char **argv) {
    setlocale(LC_CTYPE, "");
    unsigned int i, j;
    struct pgm_file pgm;
    FILE *file = fopen("baboon_coloured.pgm", "rb");
    //FILE *file = fopen("lenochka.ppm", "rb");
    if (!file) {
        printf("Can`t open input file\n");
        return -1;
    }

    if (fscanf(file, "P%d", &pgm.format)==0) {
        printf("Please check the format of the file\n");
        return -1;
    }
    fscanf(file, "%d %d", &pgm.width, &pgm.height);
    fscanf(file, "%d\n", &pgm.colours);
    unsigned char *p = malloc(sizeof(unsigned char) * 3 * pgm.height * pgm.width);
    if (p!=NULL) {
        printf("Can`t free my mind for p\n");
        return -1;
    }
    fread(p, sizeof(char), 3 * (pgm.height * pgm.width), file);

    FILE *f1 = fopen("D:\\labs\\geom\\0\\cmake-build-debug\\readed_data.txt", "r+");
    if (!f1) {
        printf("Can`t open output file\n");
        return -1;
    }
    fprintf(f1, "%d\n", pgm.height);
    fprintf(f1, "%d\n", pgm.width);
    printf("printed in file\n");
    unsigned char *p_new_reverse = malloc(sizeof(unsigned char) * 3 * pgm.height * pgm.width);
    if (p_new_reverse!=NULL) {
        printf("Can`t free my mind for p_new_reverse\n");
        return -1;
    }
    for (i = 0; i < pgm.height; ++i) {
        for (j = 0; j < pgm.width * 3; j += 3) {
            p_new_reverse[i * pgm.width * 3 + pgm.width * 3 - j - 3] = p[i * pgm.width * 3 + j + 0];
            p_new_reverse[i * pgm.width * 3 + pgm.width * 3 - j - 2] = p[i * pgm.width * 3 + j + 1];
            p_new_reverse[i * pgm.width * 3 + pgm.width * 3 - j - 1] = p[i * pgm.width * 3 + j + 2];
        }
    }

    fprintf(f1, "%d %d %d\n", p[215 * pgm.width * 3 + 500 + 0], p[215 * pgm.width * 3 + 500 + 1],
            p[215 * pgm.width * 3 + 500 + 2] );
    fprintf(f1, "%d %d %d\n", p_new_reverse[215 * pgm.width * 3 + 500 + 0],
            p_new_reverse[215 * pgm.width * 3 + 500 + 1], p_new_reverse[215 * pgm.width * 3 + 500 + 2] );

    unsigned char *p_new_reverse_inverse = malloc(sizeof(unsigned char) * 3 * pgm.height * pgm.width);
    if (p_new_reverse_inverse!=NULL) {
        printf("Can`t free my mind for p_new_reverse_inverse\n");
        return -1;
    }
    int k = 0;
    for (i = 0; i < pgm.height; ++i) {
        for (j = 0; j < pgm.width * 3; ++j) {
            p_new_reverse_inverse[i * 3 * pgm.width + j] = 255 - p_new_reverse[i * 3 * pgm.width + j];
            k++;
        }
    }

    printf("%d\n", k);
    FILE *file_new = fopen("baboon_coloured_new.pgm", "wb");
    //FILE *file_new = fopen("lenochka_new.ppm", "wb");
    fprintf(file_new, "P%d\n", pgm.format);
    fprintf(file_new, "%d %d\n", pgm.width, pgm.height);
    fprintf(file_new, "%d\n", pgm.colours);
    fwrite(p_new_reverse, sizeof(char), (3 * pgm.height * pgm.width), file_new);
    fclose(f1);
    fclose(file);
    fclose(file_new);
}