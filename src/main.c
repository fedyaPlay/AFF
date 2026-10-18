#include "aff_library.h"
#include <stdio.h> 

int main() {
AffConfig config;
config.target_core = 1;
config.use_background = 1; 

printf("Testing AFF compression...\n");
int compress_result = aff_compress_file("input.txt", "output.aff", &config);
if (compress_result != 0) {
printf("Compression failed with error: %d\n", compress_result);
return -1;
}
printf("Compression successful.\n");

printf("Testing AFF decompression...\n");
int decompress_result = aff_decompress_file("output.aff", "restored.txt");
if (decompress_result != 0) {
printf("Decompression failed with error: %d\n", decompress_result);
return -1;
}
printf("Decompression successful.\n");

return 0;

}