#ifndef RLE_H
#define RLE_H

#include <stdint.h>
#include <stddef.h>

/* 
 * В худшем случае (если данные вообще не повторяются)
 * RLE увеличит размер данных ровно в два раза.
 */
#define RLE_MAX_COMPRESSED_SIZE(input_size) ((input_size) * 2)

/**
 * @brief Сжимает исходный буфер по алгоритму RLE.
 * @param input Указатель на исходные данные.
 * @param input_size Размер исходных данных в байтах.
 * @param output Указатель на заранее выделенный буфер для результата.
 * @return size_t Итоговый размер сжатых данных в байтах.
 */
size_t AFF_Compress_RLE(const uint8_t *input, size_t input_size, uint8_t *output);

/**
 * @brief Распаковывает RLE-данные обратно в исходный вид.
 * @param compressed Указатель на сжатые данные.
 * @param compressed_size Размер сжатых данных в байтах.
 * @param output Указатель на заранее выделенный буфер для распакованных данных.
 * @return size_t Итоговый размер распакованных данных в байтах.
 */
size_t AFF_Decompress_RLE(const uint8_t *compressed, size_t compressed_size, uint8_t *output);

#endif /* RLE_H */
