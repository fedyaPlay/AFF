#ifndef IO_H
#define IO_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Читает файл целиком в память.
 * @param filename Путь к файлу.
 * @param out_size Указатель, куда запишется размер прочитанных данных.
 * @return uint8_t* Указатель на выделенный буфер с данными (нужно очистить через free()).
 *         Возвращает NULL в случае ошибки.
 */
uint8_t* AFF_Read_File(const char *filename, size_t *out_size);

/**
 * @brief Записывает буфер данных в файл на диск.
 * @param filename Путь к создаваемому файлу.
 * @param data Указатель на буфер с данными.
 * @param data_size Размер буфера в байтах.
 * @return int Возвращает 1 при успехе, 0 при ошибке.
 */
int AFF_Write_File(const char *filename, const uint8_t *data, size_t data_size);

#endif /* IO_H */
