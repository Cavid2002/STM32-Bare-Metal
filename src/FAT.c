#include "../include/FAT.h"
#include "../include/SD.h"
#include <string.h>
#include <stdlib.h>

super_block s_block;
uint8_t sector_buff[BLOCK_SIZE];

uint32_t alloc_block()
{
    uint32_t* blk_buff = sector_buff;
    uint32_t start = s_block.free_block_num;
    uint32_t stop = s_block.total_block_number;
    uint32_t block_offset, index;
    int i, j;
    for(i = start; i < stop; i++)
    {
        block_offset = i / ADR_PER_BLOCK;
        index = i % ADR_PER_BLOCK;
        SD_read_block(sector_buff, block_offset + FAT_START);
        for(j = index; j < ADR_PER_BLOCK; j++)
        {
            if(blk_buff[j] == 0)
            {
                blk_buff[j] = 0xFFFFFFFF;
                SD_write_block(blk_buff, block_offset);
                return j + block_offset * ADR_PER_BLOCK;
            }
        }
    }

    return NULL_BLCK;
}


void add_to_cluster(uint32_t prev, uint32_t next)
{
    uint32_t* blk_buff = sector_buff;
    uint32_t block_offset = prev / ADR_PER_BLOCK;
    uint32_t index = prev % ADR_PER_BLOCK;

    SD_read_block(blk_buff, block_offset + FAT_START);
    blk_buff[index] = next;
    SD_write_block(blk_buff, block_offset + FAT_START);    
}

uint32_t get_next_block(uint32_t block_num)
{
    uint32_t* blk_arr = sector_buff;
    uint32_t block_offset = block_num / ADR_PER_BLOCK;
    uint32_t index = block_num % ADR_PER_BLOCK;
    
    SD_read_block(blk_arr, FAT_START + block_offset);
    return blk_arr[index];
}

uint32_t file_seek(file_desc* fd, uint8_t pos, int offset)
{
    if(pos == SEEK_CUR) fd->offset += offset;    
    else if(pos == SEEK_SET) fd->offset = offset;
    else if(pos == SEEK_END) fd->offset = fd->file_size + offset;
    
    if(fd->offset > fd->file_size) fd->offset = fd->file_size;

    fd->curr_block = fd->start_block;

    for(int i = 0; i < fd->offset / BLOCK_SIZE; i++)
    {
        fd->curr_block = get_next_block(fd->curr_block);
    }

    return fd->offset;
}


int file_read(file_desc* fd, char* buff, uint32_t size)
{
    uint32_t internal_offset ,bytes_to_read;
    uint32_t next_block = fd->curr_block;
    if(fd->offset + size > fd->file_size) 
        size = fd->file_size - fd->offset;   
    
    if(fd->offset == fd->file_size) 
        return 0;

    
    int res = 0;
    while(1)
    {
        internal_offset = fd->offset % BLOCK_SIZE;
        bytes_to_read = BLOCK_SIZE - internal_offset;
        if(size < bytes_to_read) bytes_to_read = size;
        
        SD_read_block(sector_buff, next_block);
        memcpy(buff, sector_buff + internal_offset, bytes_to_read);
        
        buff += bytes_to_read;
        size -= bytes_to_read;
        fd->offset += bytes_to_read;
        res += bytes_to_read;

        if(size == 0) break;
        
        next_block = get_next_block(next_block);
    }
    
    fd->curr_block = next_block;
    return res;
}


int file_write(file_desc* fd, char* buff, uint32_t size)
{
    uint32_t internal_offset ,bytes_to_write;
    uint32_t next_block = fd->curr_block;
    uint32_t temp;

    int res = 0;
    while(1)
    {
        internal_offset = fd->offset % BLOCK_SIZE;
        bytes_to_write = BLOCK_SIZE - internal_offset;
        if(size < bytes_to_write) bytes_to_write = size;
        
        if(internal_offset != 0) SD_read_block(sector_buff, next_block);
        memcpy(sector_buff, buff + internal_offset, bytes_to_write);
        SD_write_block(sector_buff, next_block);
        
        buff += bytes_to_write;
        size -= bytes_to_write;
        fd->offset += bytes_to_write;
        res += bytes_to_write;

        if(size == 0) break;

        temp = get_next_block(next_block);        
        if(temp == EOC)
        {
            temp = alloc_block();
            add_to_cluster(next_block, temp);
            temp = next_block;
            continue;
        }
        
        next_block = temp;

    }
    
    return res;
}

uint32_t read_dir(uint32_t block_num, char* file)
{
    dir_entry* dirs = sector_buff;
    while(block_num != EOC)
    {
        SD_read_block(dirs, block_num);
        
        for(int i = 0; i < ENT_PER_BLOCK; i++)
        {
            if(strncmp(dirs[i].name, file) == 0)
            {
                return dirs[i].block_num;
            }
        }

        block_num = get_next_block(block_num);
        
    }

    return FSYS_ERR_NOT_EXT;
}

file_desc* file_open(char* path, uint8_t perms, uint8_t flags)
{
    char* token = strtok(path, "/");
    uint32_t next = ROOT_DIR;
    while(token)
    {
        next = read_dir(next, token);
        if(next)        
    }



}


int file_close(file_desc* fd)
{
    


}

int mount()
{
    
}

int create_fs()
{

}