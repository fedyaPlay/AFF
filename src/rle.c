#include "rle.h"

size_t AFF_Compress_RLE(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }

    size_t out_idx = 0;

    for (size_t i = 0; i < input_size; i++) {
        uint8_t current_byte = input[i];
        uint8_t count = 1;

        // Поиск дубликатов. Ограничение 255, чтобы счетчик занимал строго 1 байт.
        while (i + 1 < input_size && input[i + 1] == current_byte && count < 255) {
            count++;
            i++;
        }

        // Запись пары: [Количество][Байт]
        output[out_idx++] = count;
        output[out_idx++] = current_byte;
    }

    return out_idx; // Возвращаем точный размер сжатого блока
}

size_t AFF_Decompress_RLE(const uint8_t *compressed, size_t compressed_size, uint8_t *output) {
    if (compressed == NULL || output == NULL || compressed_size == 0) {
        return 0;
    }

    size_t out_idx = 0;

    // Шаг равен 2, читаем парами
    for (size_t i = 0; i < compressed_size; i += 2) {
        uint8_t count = compressed[i];
        uint8_t current_byte = compressed[i + 1];

        // Восстановление цепочки байт
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = current_byte;
        }
    }

    return out_idx; // Возвращаем размер восстановленных данных
}
