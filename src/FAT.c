#include "../include/FAT.h"
#include "../include/SD.h"
#include <string.h>
#include <stdlib.h>

super_block s_block;


int (*disk_read)(char*, uint32_t, uint16_t) = SD_read;
int (*disk_write)(char*, uint32_t, uint16_t) = SD_write; 
int (*disk_write_sync)(char*, uint32_t, uint16_t) = SD_write_sync;

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
        disk_read(sector_buff, block_offset + FAT_START);
        for(j = index; j < ADR_PER_BLOCK; j++)
        {
            if(blk_buff[j] == 0)
            {
                blk_buff[j] = EOC;
                disk_write(blk_buff, block_offset + FAT_START);
                return j + block_offset * ADR_PER_BLOCK;
            }
        }
    }

    return FAT_ERR_NO_SPC;
}


void add_to_cluster(uint32_t prev, uint32_t next)
{
    uint32_t* blk_buff = sector_buff;
    uint32_t block_offset = prev / ADR_PER_BLOCK;
    uint32_t index = prev % ADR_PER_BLOCK;

    disk_read(blk_buff, block_offset + FAT_START);
    blk_buff[index] = next;
    disk_write(blk_buff, block_offset + FAT_START);    
}

uint32_t get_next_block(uint32_t block_num)
{
    uint32_t* blk_arr = sector_buff;
    uint32_t block_offset = block_num / ADR_PER_BLOCK;
    uint32_t index = block_num % ADR_PER_BLOCK;
    
    disk_read(blk_arr, FAT_START + block_offset);
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
        
        disk_read(sector_buff, next_block);
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
        
        if(internal_offset != 0) disk_read(sector_buff, next_block);
        memcpy(sector_buff + internal_offset, buff, bytes_to_write);
        disk_write(sector_buff, next_block);
        
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
        }
        
        next_block = temp;

    }
    
    return res;
}

dir_entry read_dir(uint32_t block_num, char* filename)
{
    dir_entry* dirs = sector_buff;
    while(block_num != EOC)
    {
        disk_read(dirs, block_num);
        
        for(int i = 0; i < DIR_PER_BLOCK; i++)
        {
            if(strncmp(dirs[i].name, filename, strlen(filename)) == 0)
            {
                return dirs[i];
            }
        }

        block_num = get_next_block(block_num);
        
    }

    return (dir_entry){.name_length = 0};
}

int write_dir(uint32_t block_num, dir_entry new_entry)
{
    dir_entry* dirs = sector_buff;
    uint32_t prev_block = block_num;

    while(block_num != EOC)
    {
        disk_read(dirs, block_num);
        for(int i = 0; i < DIR_PER_BLOCK; i++)
        {
            if(dirs[i].name_length == 0 && dirs[i].block_num == 0)
            {
                dirs[i] = new_entry;
                disk_write(dirs, block_num, BLOCK_SIZE);
                return 0;
            }
        }
        prev_block = block_num;
        block_num = get_next_block(block_num);
    }

    block_num = alloc_block();
    if(block_num == FAT_ERR_NO_SPC) return FAT_ERR_NO_SPC;
    
    add_to_cluster(prev_block, block_num);
    disk_write(&new_entry, block_num, sizeof(new_entry));
    return new_entry;
}

file_desc file_open(char* path, uint8_t flags)
{
    char* token = strtok(path, "/");
    char* filename = token;
    dir_entry temp;
    uint32_t next = ROOT_DIR;
    uint32_t prev = next;
    while(token)
    {
        temp = read_dir(next, token);
        if(temp.name_length == 0) break;
        prev = next;
        next = temp.block_num;
        filename = token;
        token = strtok(NULL, "/");
    }
    

    if(strtok(NULL, "/") != NULL) return FAT_ERR_PATH_ERR;

    if(flags == FAT_FLAG_CREATE)
    {
        temp.block_num = alloc_block();
        temp.name_length = strlen(filename);
        temp.file_size = 0;

        strncpy(temp.name, filename, temp.name_length);
        write_dir(prev, temp);
    } 
    
    file_desc fd;
    fd.curr_block = temp.block_num;
    fd.start_block = temp.block_num;
    fd.file_size = temp.file_size;
    fd.dir_entry_block = prev;
    fd.offset = 0;

    return fd;
}


int file_close(file_desc* fd)
{
    dir_entry* dirs = sector_buff;
    uint32_t next = fd->dir_entry_block;
    while(next != EOC)
    {
        SD_read(dirs, next, BLOCK_SIZE);
        for(int i = 0; i < DIR_PER_BLOCK; i++)
        {
            if(dirs[i].block_num == fd->start_block)
            {
                dirs[i].file_size = fd->file_size;
                SD_write(dirs, next, BLOCK_SIZE);
                return 0;
            }
        }
        next = get_next_block(next);
    }

    return 0;
}

int mount()
{
    SD_read(&s_block, 0, sizeof(s_block));
}

int create_fs(uint32_t max_block_num)
{
    memset(sector_buff, 0, BLOCK_SIZE);
    s_block.block_size = BLOCK_SIZE;
    s_block.total_block_number = max_block_num;
    s_block.fat_table_size = (max_block_num * 4 + BLOCK_SIZE) / BLOCK_SIZE;
    s_block.root_dir = s_block.fat_table_size + 1;
    s_block.free_blocks = max_block_num - s_block.fat_table_size - 1;

    for(int i = 0; i < s_block.fat_table_size + 1; i++)
    {
        disk_write(sector_buff, i, BLOCK_SIZE);
    }

    disk_write(&s_block, 0, sizeof(s_block));
    
}
