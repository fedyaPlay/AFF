#ifndef AFF_LIBRARY_H
#define AFF_LIBRARY_H 

#include <stdint.h>
#include <stddef.h> 

#define AFF_MAGIC 0x21464641 

#if defined(_WIN32)
#define AFF_API __declspec(dllexport)
#else
#define AFF_API **attribute**((visibility("default")))
#endif 

typedef struct {
int target_core;
int use_background;
} AffConfig; 

typedef struct **attribute**((packed)) {
uint32_t file_name_hash;
uint64_t compressed_size;
uint64_t original_size;
uint32_t data_crc32;
uint16_t name_length;
} AffFileHeader; 

AFF_API int aff_compress_file(const char* input_path, const char* output_path, AffConfig* config);
AFF_API int aff_decompress_file(const char* input_path, const char* output_path); 

#endif