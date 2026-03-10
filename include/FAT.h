#ifndef FAT_H
#define FAT_H

#include <stdint.h>

#define FILE_NAME_MAX           20
#define BLOCK_SIZE              512
#define FAT_START               2
#define ADR_PER_BLOCK           BLOCK_SIZE / 4
#define EOC                     0xFFFFFFFF

#define SEEK_SET                0
#define SEEK_CUR                1
#define SEEK_END                2


#define NULL_BLCK   0

typedef struct
{
    uint32_t fsys_id;
    uint32_t block_size;
    uint32_t total_block_number;
    uint32_t free_block_num;
} __attribute__((packed)) super_block;



typedef struct
{
    uint32_t block_num;
    uint32_t file_size;
    uint8_t type;
    uint8_t perms;
    uint8_t owner_id;
    char name[20];
} __attribute__((packed)) dir_entry;


typedef struct
{
    uint32_t file_size;
    uint32_t offset;
    uint32_t start_block;
    uint32_t curr_block;
} file_desc;



#endif