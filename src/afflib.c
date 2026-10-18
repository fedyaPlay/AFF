#include "aff_library.h"
#include <stdio.h>
#include <stdlib.h> 

#if defined(_WIN32)
#include <windows.h>
#else
#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <sys/resource.h>
#endif 

extern uint32_t asm_crc32(const uint8_t* data, size_t length);
extern uint32_t asm_hash_name(const char* name); 

void apply_affinity_settings(AffConfig* config) {
if (!config) return; 

#if defined(_WIN32)
if (config->target_core >= 0) {
DWORD_PTR mask = (DWORD_PTR)1 << config->target_core;
SetThreadAffinityMask(GetCurrentThread(), mask);
}
if (config->use_background) {
SetThreadPriority(GetCurrentThread(), THREAD_MODE_BACKGROUND_BEGIN);
}
#else
if (config->target_core >= 0) {
cpu_set_t cpuset;
CPU_ZERO(&cpuset);
CPU_SET(config->target_core, &cpuset);
sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
}
if (config->use_background) {
setpriority(PRIO_PROCESS, 0, 19);
}
#endif
} 

AFF_API int aff_compress_file(const char* input_path, const char* output_path, AffConfig* config) {
apply_affinity_settings(config); 

FILE* in = fopen(input_path, "rb");
FILE* out = fopen(output_path, "wb");
if (!in || !out) return -1;

uint32_t magic = AFF_MAGIC;
fwrite(&magic, sizeof(magic), 1, out);

fclose(in);
fclose(out);

#if defined(_WIN32)
if (config && config->use_background) {
SetThreadPriority(GetCurrentThread(), THREAD_MODE_BACKGROUND_END);
}
#endif
return 0;
} 

AFF_API int aff_decompress_file(const char* input_path, const char* output_path) {
FILE* in = fopen(input_path, "rb");
FILE* out = fopen(output_path, "wb");
if (!in || !out) return -1; 

uint32_t magic = 0;
fread(&magic, sizeof(magic), 1, in);
if (magic != AFF_MAGIC) {
fclose(in);
fclose(out);
return -2;
}

fclose(in);
fclose(out);
return 0;

}