#include "../include/FAT.h"
#include "../include/SD.h"
#include <string.h>

super_block s_block;
uint8_t sector_buff[BLOCK_SIZE];

uint32_t alloc_block()
{
    ...
}

uint32_t get_next_block(uint32_t block_num)
{
    uint32_t* blk_arr = sector_buff;
    uint32_t fat_start = FAT_TABLE_START_ADDR;
    uint32_t block_offset = block_num / ADR_PER_BLOCK;
    uint32_t index = block_num % ADR_PER_BLOCK;
    
    SD_read_block(blk_arr, fat_start + block_offset);
    return blk_arr[index];
}


int read_file(file_desc* fd, char* buff, uint32_t size)
{
    uint32_t block_offset, internal_offset;
    uint32_t next_block = fd->start_block;
    if(fd->offset + size > fd->file_size) 
        size = fd->file_size - fd->offset;   
    
    if(fd->offset == fd->file_size) 
        return 0;

    
    block_offset = fd->offset / BLOCK_SIZE;
    internal_offset = fd->offset % BLOCK_SIZE;


    for(int i = 0; i < block_offset; i++)
    {
        next_block = 
    }

}


int write_file(file_desc* fd, char* buff, uint32_t size)
{
    
}