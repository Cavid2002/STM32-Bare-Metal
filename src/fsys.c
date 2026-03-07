#include "../include/fsys.h"
#include "../include/SD.h"

uint8_t sector_buff[SECTOR_SIZE];
 

uint32_t get_block(inode* in, uint32_t block_offset)
{
    uint32_t* block_buff = sector_buff;
    uint32_t index = block_offset;
    if(index < DIRECT_BLOCK)
    {
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
    uint32_t block_offset;
    if(fd->offset + size > fd->in.size) size = fd->in.size - fd->offset;
    
    

    
    
    

}