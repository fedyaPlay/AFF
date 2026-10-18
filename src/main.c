#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rle.h"
#include "io.h"

// Strictly aligned AFF format metadata header
// Total overhead size before payload data is exactly 16 bytes
#pragma pack(push, 1)
typedef struct {
    char magic[4];           // 4 bytes: Format signature, explicitly "AFF!"
    uint32_t original_size;   // 4 bytes: Size of the original file before RLE compression
    uint32_t compressed_size; // 4 bytes: Size of the payload after RLE compression
    uint32_t flags;           // 4 bytes: Reserved options for future extension
} AFF_Header;
#pragma pack(pop)

// Displays application CLI manual
void print_usage(const char *prog_name) {
    printf("AFF Archiver Usage:\n");
    printf("  Compression: %s pack   <input_file> <output_archive.aff>\n", prog_name);
    printf("  Extraction:  %s unpack <input_archive.aff> <output_file>\n", prog_name);
}

// Core routine for packaging files into the AFF archive
int handle_pack(const char *input_file, const char *output_archive) {
    size_t orig_size = 0;
    
    // 1. Read input asset into RAM
    uint8_t *orig_data = AFF_Read_File(input_file, &orig_size);
    if (!orig_data) {
        fprintf(stderr, "Error: Failed to read target file: %s\n", input_file);
        return 1;
    }

    // 2. Allocate scratch space for compression bounds
    size_t max_compressed = RLE_MAX_COMPRESSED_SIZE(orig_size);
    uint8_t *compressed_data = (uint8_t *)malloc(max_compressed);
    if (!compressed_data) {
        fprintf(stderr, "Error: Failed to allocate memory block for compression\n");
        free(orig_data);
        return 1;
    }

    // 3. Process data streams using RLE encoder
    size_t comp_size = AFF_Compress_RLE(orig_data, orig_size, compressed_data);

    // 4. Assemble standard 16-byte metadata block
    AFF_Header header;
    memcpy(header.magic, "AFF!", 4);
    header.original_size = (uint32_t)orig_size;
    header.compressed_size = (uint32_t)comp_size;
    header.flags = 0; // Reserved field initialization

    // 5. Build continuous binary layout: Header (16 bytes) + Payload
    size_t total_archive_size = sizeof(AFF_Header) + comp_size;
    uint8_t *final_archive_buffer = (uint8_t *)malloc(total_archive_size);
    
    if (!final_archive_buffer) {
        fprintf(stderr, "Error: Failed to assemble sequential archive structure in RAM\n");
        free(compressed_data);
        free(orig_data);
        return 1;
    }

    // Write header segment followed directly by compressed sequence
    memcpy(final_archive_buffer, &header, sizeof(AFF_Header));
    memcpy(final_archive_buffer + sizeof(AFF_Header), compressed_data, comp_size);

    // 6. Write final image stream to persistent storage
    if (!AFF_Write_File(output_archive, final_archive_buffer, total_archive_size)) {
        fprintf(stderr, "Error: Failed to write archive file %s to storage media\n", output_archive);
        final_archive_buffer = NULL;
        free(compressed_data);
        free(orig_data);
        return 1;
    }

    printf("Success: Pack completed into %s\n", output_archive);
    printf("Original payload: %zu bytes | Total archive output: %zu bytes\n", orig_size, total_archive_size);

    // Resource lifecycle deallocation
    free(final_archive_buffer);
    free(compressed_data);
    free(orig_data);
    return 0;
}

// Core routine for unpacking files from the AFF archive
int handle_unpack(const char *input_archive, const char *output_file) {
    size_t archive_size = 0;

    // 1. Fetch raw archive package from system storage
    uint8_t *archive_data = AFF_Read_File(input_archive, &archive_size);
    if (!archive_data) {
        fprintf(stderr, "Error: Failed to read archive file: %s\n", input_archive);
        return 1;
    }

    // 2. Validate structural frame consistency
    if (archive_size < sizeof(AFF_Header)) {
        fprintf(stderr, "Error: Malformed archive profile or invalid structure length\n");
        free(archive_data);
        return 1;
    }

    // 3. Decode operational header metadata
    AFF_Header header;
    memcpy(&header, archive_data, sizeof(AFF_Header));

    // Verify system magic bytes identity
    if (memcmp(header.magic, "AFF!", 4) != 0) {
        fprintf(stderr, "Error: Target is not a recognized AFF format archive signature\n");
        free(archive_data);
        return 1;
    }

    // Match raw physical boundary data limits
    if (archive_size != sizeof(AFF_Header) + header.compressed_size) {
        fprintf(stderr, "Error: Size verification failure between stream metadata and hardware properties\n");
        free(archive_data);
        return 1;
    }

    // 4. Request exact runtime extraction block size mapped from header parameters
    uint8_t *decompressed_data = (uint8_t *)malloc(header.original_size);
    if (!decompressed_data) {
        fprintf(stderr, "Error: Extraction failure during staging allocation\n");
        free(archive_data);
        return 1;
    }

    // 5. Decode RLE payload using index offset skip
    uint8_t *compressed_content_ptr = archive_data + sizeof(AFF_Header);
    size_t restored_size = AFF_Decompress_RLE(compressed_content_ptr, header.compressed_size, decompressed_data);

    // Validate size checksum uniformity
    if (restored_size != header.original_size) {
        fprintf(stderr, "Error: Recovered payload dimension mismatch against specification records\n");
        free(decompressed_data);
        free(archive_data);
        return 1;
    }

    // 6. Push verified binary buffer out to local file node
    if (!AFF_Write_File(output_file, decompressed_data, restored_size)) {
        fprintf(stderr, "Error: Target target export routing failed for: %s\n", output_file);
        free(decompressed_data);
        free(archive_data);
        return 1;
    }

    printf("Success: Extraction successfully executed into %s\n", output_file);
    printf("Restored file data: %zu bytes.\n", restored_size);

    // Clean execution resources
    free(decompressed_data);
    free(archive_data);
    return 0;
}

int main(int argc, char *argv[]) {
    // Validate operational runtime CLI parameters
    if (argc < 4) {
        print_usage(argv[0]);
        return 1;
    }

    const char *command = argv[1];

    if (strcmp(command, "pack") == 0) {
        return handle_pack(argv[2], argv[3]);
    } 
    else if (strcmp(command, "unpack") == 0) {
        return handle_unpack(argv[2], argv[3]);
    } 
    else {
        fprintf(stderr, "Error: Operation command string unrecognized: '%s'\n", command);
        print_usage(argv[0]);
        return 1;
    }
}
