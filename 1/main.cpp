#include <stdlib.h> // <> -лучше и быстрее
#include <stdio.h>

struct pgm_file {
    unsigned int format;
    unsigned int width;
    unsigned int height;
    unsigned int colours;
};


void inverse(unsigned char *data, unsigned char *dataNew, int height, int width) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            dataNew[i * width + j] = 255 - data[i * width + j];
        }
    }
}

void verticalReflection(unsigned char *data, unsigned char *dataNew, int height, int width) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            dataNew[(height - i - 1) * width + j] = data[i * width + j];
        }
    }
}

void horizontalReflectionColoured(unsigned char *data, unsigned char *dataNew, int height, int width) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; j += 3) {
            dataNew[i * width + width - j - 3] = data[i * width + j + 0];
            dataNew[i * width + width - j - 2] = data[i * width + j + 1];
            dataNew[i * width + width - j - 1] = data[i * width + j + 2];
        }
    }
}

void horizontalReflectionBlackWhite(unsigned char *data, unsigned char *dataNew, int height, int width) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            dataNew[i * width + width - j - 1] = data[i * width + j];
        }
    }
}

void rightTurn90DegreesBlackWhite(unsigned char *data, unsigned char *dataNew, int height, int width) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            dataNew[height * j + height - i - 1] = data[i * width + j];
        }
    }
}

void rightTurn90DegreesColoured(unsigned char *data, unsigned char *dataNew, int height, int width) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            dataNew[(height * j + height - i) * 3 - 3] = data[(i * width + j) * 3 + 0];
            dataNew[(height * j + height - i) * 3 - 2] = data[(i * width + j) * 3 + 1];
            dataNew[(height * j + height - i) * 3 - 1] = data[(i * width + j) * 3 + 2];
        }
    }
}

void leftTurn90DegreesBlackWhite(unsigned char *data, unsigned char *dataNew, int height, int width) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            dataNew[(width - j - 1) * height + i] = data[i * width + j];
        }
    }
}

void leftTurn90DegreesColoured(unsigned char *data, unsigned char *dataNew, int height, int width) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            dataNew[((width - j - 1) * height + i) * 3 + 0] = data[(i * width + j) * 3 + 0];
            dataNew[((width - j - 1) * height + i) * 3 + 1] = data[(i * width + j) * 3 + 1];
            dataNew[((width - j - 1) * height + i) * 3 + 2] = data[(i * width + j) * 3 + 2];
        }
    }
}


int main(int argc, char **argv) {
    unsigned int i, j;
    int formatVariable;

    if (argc != 4) {
        printf("Please check the format of the input data\n");
        return -1;
    }

    struct pgm_file pgm;
    FILE *file = fopen(argv[1], "rb");
    if (file == nullptr) {
        printf("Can`t open input file\n");
        return -1;
    }

    FILE *file_new = fopen(argv[2], "rb");
    if (file_new == nullptr) {
        printf("Can`t open output file\n");
        fclose(file);
        return -1;
    }

    if (fscanf(file, "P%d", &pgm.format) == 0) {
        printf("Please check the format of the file\n");
        fclose(file);
        fclose(file_new);
        return -1;
    }

    fscanf(file, "%d %d", &pgm.width, &pgm.height);
    fscanf(file, "%d\n", &pgm.colours);

    if ((pgm.height < 1) || (pgm.width < 1)) {
        printf("Please check the height or width in the FrameHeader\n");
        fclose(file);
        fclose(file_new);
        return -1;
    }
    if (pgm.colours != 255) {
        printf("Please check the colours in the FrameHeader\n");
        fclose(file);
        fclose(file_new);
        return -1;
    }

    switch (pgm.format) {
        case 5:
            formatVariable = 1;
            break;
        case 6:
            formatVariable = 3;
            break;
        default:
            printf("Please check the format of the file -> (P-something)\n");
            fclose(file);
            fclose(file_new);
            return -1;
    }

    unsigned char *data = (unsigned char *) malloc(sizeof(unsigned char) * formatVariable * pgm.height * pgm.width);
    if (data == NULL) {
        printf("Can`t free my mind for data array\n");
        fclose(file);
        fclose(file_new);
        return -1;
    }
    int troubleLess = fread(data, sizeof(char), formatVariable * (pgm.height * pgm.width), file);
    if (troubleLess < formatVariable * (pgm.height * pgm.width)) {
        printf("Please check the number of the pixels: IT IS NOT ENOUGH\n");
        fclose(file);
        fclose(file_new);
        free(data);
        return -1;
    }
    int troubleMore = fread(data, sizeof(char), 1, file);
    if (troubleMore > 0) {
        printf("Please check the number of the pixels: OH GOD, IT IS TOO MUCH\n");
        fclose(file);
        fclose(file_new);
        free(data);
        return -1;
    }

    unsigned char *dataNew = (unsigned char *) malloc(sizeof(unsigned char) * formatVariable * pgm.height * pgm.width);
    if (dataNew == NULL) {
        printf("Can`t free my mind for dataNew array\n");
        fclose(file);
        fclose(file_new);
        free(data);
        return -1;
    }

    switch (atoi(argv[3])) {
        case 0:
            inverse(data, dataNew, pgm.height, pgm.width * formatVariable);
            break;
        case 1:
            if (pgm.format == 5) {
                horizontalReflectionBlackWhite(data, dataNew, pgm.height, pgm.width * formatVariable);
            } else {
                horizontalReflectionColoured(data, dataNew, pgm.height, pgm.width * formatVariable);
            }
            break;
        case 2:
            verticalReflection(data, dataNew, pgm.height, pgm.width * formatVariable);
            break;
        case 3:
            if (pgm.format == 5) {
                rightTurn90DegreesBlackWhite(data, dataNew, pgm.height, pgm.width);
            } else {
                rightTurn90DegreesColoured(data, dataNew, pgm.height, pgm.width);
            }
            break;
        case 4:
            if (pgm.format == 5) {
                leftTurn90DegreesBlackWhite(data, dataNew, pgm.height, pgm.width);
            } else {
                leftTurn90DegreesColoured(data, dataNew, pgm.height, pgm.width);
            }
            break;
        default:
            printf("I don't know what to do please help\n");
            fclose(file);
            fclose(file_new);
            free(data);
            free(dataNew);
            return -1;
    }

    file_new = fopen(argv[2], "wb");

    fprintf(file_new, "P%d\n", pgm.format);
    if (atoi(argv[3]) < 3) {
        fprintf(file_new, "%d %d\n", pgm.width, pgm.height);
    } else {
        fprintf(file_new, "%d %d\n", pgm.height, pgm.width);
    }
    fprintf(file_new, "%d\n", pgm.colours);
    fwrite(dataNew, sizeof(char), (formatVariable * pgm.height * pgm.width), file_new);
    fclose(file);
    fclose(file_new);
    free(data);
    free(dataNew);
}