#include "io.h"
#include <stdio.h>
#include <stdlib.h>

uint8_t* AFF_Read_File(const char *filename, size_t *out_size) {
    if (filename == NULL || out_size == NULL) return NULL;

    // Открываем файл строго в бинарном режиме ("rb")
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return NULL; 
    }

    // Определяем размер файла
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    if (size < 0) {
        fclose(file);
        return NULL;
    }
    fseek(file, 0, SEEK_SET);

    *out_size = (size_t)size;

    // Выделяем память под размер файла
    uint8_t *buffer = (uint8_t *)malloc(*out_size);
    if (buffer == NULL) {
        fclose(file);
        return NULL;
    }

    // Считываем данные в буфер
    size_t read_bytes = fread(buffer, 1, *out_size, file);
    fclose(file);

    // Проверяем, всё ли прочиталось
    if (read_bytes != *out_size) {
        free(buffer);
        return NULL;
    }

    return buffer;
}

int AFF_Write_File(const char *filename, const uint8_t *data, size_t data_size) {
    if (filename == NULL || (data == NULL && data_size > 0)) return 0;

    // Открываем файл на запись в бинарном режиме ("wb")
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        return 0;
    }

    // Записываем весь буфер за один раз
    size_t written_bytes = fwrite(data, 1, data_size, file);
    fclose(file);

    // Если записалось столько же, сколько просили — успех
    return (written_bytes == data_size);
}
