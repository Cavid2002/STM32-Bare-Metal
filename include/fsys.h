#ifndef FSYS_H
#define FSYS_H

#include <stdint.h>
#include "../include/SD.h"

#define FILE_NAME_MAX  26
#define INODE_SIZE     32
#define BITS_PER_BLOCK (BLOCK_SIZE << 3)

#define DIRECT_BLOCK    3
#define SINGLE_INDIRECT_BLOCK  (BLOCK_SIZE / 4)
#define DOUBLE_INDIRECT_BLOCK  SINGLE_INDIRECT_BLOCK * (BLOCK_SIZE / 4)
#define TRIPLE_INDIRECT_BLOCK  DOUBLE_INDIRECT_BLOCK * (BLOCK_SIZE / 4)

#define NULL_BLCK   0

typedef struct
{
    uint32_t fsys_id;
    uint32_t block_size;
    uint32_t total_block_number;
    uint32_t total_inode_num;
    uint32_t block_bitmap_addr;
    uint32_t inode_bitmap_addr;
    uint32_t inode_table_addr;
    uint32_t free_inode_num;
    uint32_t free_block_num;
} __attribute__((packed)) super_block;

typedef struct
{
    uint8_t type;
    uint8_t perm;
    uint16_t user_id;
    uint32_t size;
    uint32_t direct[3];
    uint32_t single_indirect;
    uint32_t double_indirect;
    uint32_t triple_indirect;
} __attribute__((packed)) inode;


typedef struct
{
    uint32_t inode_num;
    uint8_t name_length;
    uint8_t type;
    char name[26];
} __attribute__((packed)) dir_entry;


typedef struct
{
    inode in;
    uint32_t offset;
    uint32_t inode_num;
} file_desc;



#endif