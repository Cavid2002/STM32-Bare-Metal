#include "../include/fsys.h"
#include "../include/SD.h"
#include <string.h>

super_block s_block;
uint32_t bitmap_buff[BLOCK_SIZE / 4];

uint32_t alloc_block()
{
    
    uint32_t start = s_block.free_block_num; 
    uint32_t stop = sb.total_block_number;
    uint32_t block_num, bmp_offset;
    for(int i = start; i < stop; i++)
    {
        bmp_offset = i % BITS_PER_BLOCK;
        block_num = i / BITS_PER_BLOCK;
        
        
        SD_read_block(bitmap_buff, s_block.block_bitmap_addr + block_num);
        
        for(int i = 0; i < BITS_PER_BLOCK; i++)
        {
            
        }
        
        
    }
}


uint32_t get_block(inode* in, uint32_t block_num)
{
    uint32_t* block_buff = sector_buff;
    uint32_t index = block_num;
    if(index < DIRECT_BLOCK)
    {
        if(!(in->direct[index]))
        {
            in->direct[index] = alloc_block();
        }
        return in->direct[index];
    }
    
    index -= DIRECT_BLOCK; 
    if(index < SINGLE_INDIRECT_BLOCK)
    {
        SD_read_block(block_buff, in->single_direct);
        return block_buff[index];
    }
    
    index -= SINGLE_INDIRECT_BLOCK;
    if(index < DOUBLE_INDIRECT_BLOCK)
    {
        SD_read_block(block_buff, in->double_indirect);
        uint32_t l1 = index / SINGLE_INDIRECT_BLOCK;
        uint32_t l2 = index % SINGLE_INDIRECT_BLOCK;

        SD_read_block(block_buff, block_buff[l1]);
        return block_buff[l2];
    }

    index -= DOUBLE_INDIRECT_BLOCK;
    if(index < TRIPLE_INDIRECT_BLOCK)
    {
        SD_read_block(block_buff, in->triple_indirect);
        uint32_t l1 = index / DOUBLE_INDIRECT_BLOCK;
        uint32_t internal = index % DOUBLE_INDIRECT_BLOCK;

        SD_read_block(block_buff, block_buff[l1]);
        uint32_t l2 = index / SINGLE_INDIRECT_BLOCK;
        uint32_t l3 = index % SINGLE_INDIRECT_BLOCK;

        SD_read_block(block_buff, block_buff[l2]);
        return block_buff[l3];
    }


    return NULL_BLCK;
}

int read_file(file_desc* fd, char* buff, uint32_t size)
{
    uint32_t block_num, block_offset, block_addr;
    int bytes_to_read = 0, res = 0;

    if(fd->offset >= fd->in.size) return 0;
    if(fd->offset + size > fd->in.size) size = fd->in.size - fd->offset;

    

    while(size > 0)
    {
        block_num = fd->offset / BLOCK_SIZE;
        block_offset = fd->offset % BLOCK_SIZE;
        bytes_to_read = BLOCK_SIZE - block_offset;
        
        block_addr = get_block(&fd->in, block_num);
        SD_read_block(sector_buff, block_addr);
        memcpy(buff, sector_buff + block_offset, bytes_to_read);
        
        buff += bytes_to_read;
        size -= bytes_to_read;
        fd->offset += bytes_to_read;
        res += bytes_to_read;
    }

    return res;
}


int write_file(file_desc* fd, char* buff, uint32_t size)
{
    uint32_t block_num, block_offset, block_addr;
    int bytes_to_write = 0, res = 0;

    while(size > 0)
    {
        if()
    }
}