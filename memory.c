#include <stdio.h>
#include "oslabs.h"

struct MEMORY_BLOCK NULLBLOCK = {0,0,0,0};

void split_block(struct MEMORY_BLOCK memory_map[MAPMAX], int index, int array_length, int mem_block_size, int request_size)
{
    int size_dif = mem_block_size - request_size;
    int i;
    for(i = index; i < array_length - 1; i++) memory_map[i] = memory_map[i + 1];
}


struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id); 
struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id); 
struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id);
struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id, int last_address);
void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt);


struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id)
{
    struct MEMORY_BLOCK best_fit = NULLBLOCK;
    int best_fit_index = -1;
    for (int i = 0; i < *map_cnt; i++)
    {
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size == request_size)
        {
            memory_map[i].process_id = process_id;
            return memory_map[i];
        }
        else if (memory_map[i].process_id == 0 && memory_map[i].segment_size > request_size)
        {
            if (best_fit.segment_size == 0)
            {
                best_fit = memory_map[i];
                best_fit_index = i;
            }
            else if (memory_map[i].segment_size < best_fit.segment_size)
            {
                best_fit = memory_map[i];
                best_fit_index = i;
            }
        }
    }
    if (best_fit.segment_size == 0)
    {
        return NULLBLOCK;
    }
    else 
    {
        int size_dif = memory_map[best_fit_index].segment_size - request_size;
        memory_map[best_fit_index].segment_size = request_size;
        memory_map[best_fit_index].process_id = process_id;
        memory_map[best_fit_index].end_address = memory_map[best_fit_index].start_address + request_size - 1;
        for (int i = best_fit_index + 1; i <= *map_cnt; i++)
        {
            memory_map[i+1] = memory_map[i];
        }
        memory_map[best_fit_index + 1].process_id = 0;
        memory_map[best_fit_index + 1].segment_size = size_dif;
        memory_map[best_fit_index + 1].start_address = memory_map[best_fit_index].end_address + 1;
        memory_map[best_fit_index + 1].end_address = memory_map[best_fit_index + 1].start_address + size_dif - 1;
        *map_cnt += 1;
        best_fit = memory_map[best_fit_index];
    }
    return best_fit;
}

struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id)
{
    struct MEMORY_BLOCK first_fit = NULLBLOCK;
    for (int i = 0; i < *map_cnt; i++)
    {
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size == request_size)
        {
            memory_map[i].process_id = process_id;
            return memory_map[i];
        }
        else if (memory_map[i].process_id == 0 && memory_map[i].segment_size > request_size)
        {
            int size_dif = memory_map[i].segment_size - request_size;
            memory_map[i].segment_size = request_size;
            memory_map[i].process_id = process_id;
            memory_map[i].end_address = memory_map[i].start_address + request_size - 1;
            *map_cnt += 1;
            for (int k = *map_cnt - 1; k > i; k--)
            {
                memory_map[k] = memory_map[k-1];
            }
            memory_map[i + 1].process_id = 0;
            memory_map[i + 1].segment_size = size_dif;
            memory_map[i+ 1].start_address = memory_map[i].end_address + 1;
            memory_map[i + 1].end_address = memory_map[i + 1].start_address + size_dif - 1;
            first_fit = memory_map[i];
            return first_fit;
        }
    }
    return NULLBLOCK;
}

struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id)
{
    struct MEMORY_BLOCK worst_fit = NULLBLOCK;
    int worst_fit_index;
    for (int i = 0; i < *map_cnt; i++)
    {
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size == request_size)
        {
            memory_map[i].process_id = process_id;
            return memory_map[i];
        }
        else if (memory_map[i].process_id == 0 && memory_map[i].segment_size > request_size)
        {
            if (worst_fit.segment_size == 0)
            {
                worst_fit = memory_map[i];
                worst_fit_index = i;
            }
            else if (memory_map[i].segment_size > worst_fit.segment_size)
            {
                worst_fit = memory_map[i];
                worst_fit_index = i;
            }
        }
    }
    if (worst_fit.segment_size == 0)
    {
        return NULLBLOCK;
    }
    else 
    {
        int size_dif = memory_map[worst_fit_index].segment_size - request_size;
        memory_map[worst_fit_index].segment_size = request_size;
        memory_map[worst_fit_index].process_id = process_id;
        memory_map[worst_fit_index].end_address = memory_map[worst_fit_index].start_address + request_size - 1;
        for (int i = worst_fit_index + 1; i <= *map_cnt; i++)
        {
            memory_map[i+1] = memory_map[i];
        }
        memory_map[worst_fit_index + 1].process_id = 0;
        memory_map[worst_fit_index + 1].segment_size = size_dif;
        memory_map[worst_fit_index + 1].start_address = memory_map[worst_fit_index].end_address + 1;
        memory_map[worst_fit_index + 1].end_address = memory_map[worst_fit_index + 1].start_address + size_dif - 1;
        *map_cnt += 1;
        worst_fit = memory_map[worst_fit_index];
    }
    return worst_fit;
}

struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id, int last_address)
{
    struct MEMORY_BLOCK next_fit = NULLBLOCK;
    for (int i = last_address; i < *map_cnt; i++)
    {
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size == request_size)
        {
            memory_map[i].process_id = process_id;
            return memory_map[i];
        }
        else if (memory_map[i].process_id == 0 && memory_map[i].segment_size > request_size)
        {
            int size_dif = memory_map[i].segment_size - request_size;
            memory_map[i].segment_size = request_size;
            memory_map[i].process_id = process_id;
            memory_map[i].end_address = memory_map[i].start_address + request_size - 1;
            *map_cnt += 1;
            for (int k = i+1; k < *map_cnt; k++)
            {
                memory_map[k+1] = memory_map[k];
            }
            memory_map[i + 1].process_id = 0;
            memory_map[i + 1].segment_size = size_dif;
            memory_map[i+ 1].start_address = memory_map[i].end_address + 1;
            memory_map[i + 1].end_address = memory_map[i + 1].start_address + size_dif - 1;
            next_fit = memory_map[i];
            return next_fit;
        }
    }
    return NULLBLOCK;
}

void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt)
{
    //freed_block.process_id = 0;
    int free_block_index;
    for (int i = 0; i < *map_cnt; i++)
    {
        if (memory_map[i].start_address == freed_block.start_address)
        {
            free_block_index = i;
        }
    }
    memory_map[free_block_index].process_id = 0;
    if (memory_map[free_block_index + 1].process_id == 0)
    {
        memory_map[free_block_index].end_address = memory_map[free_block_index + 1].end_address;
        memory_map[free_block_index].segment_size = memory_map[free_block_index].segment_size + memory_map[free_block_index + 1].segment_size;
        for (int i = free_block_index + 1; i < *map_cnt; i++)
        {
            memory_map[i] = memory_map[i+1];
        }
        *map_cnt -= 1;
    }
    if (free_block_index - 1 >= 0 && memory_map[free_block_index - 1].process_id == 0)
    {
        memory_map[free_block_index].start_address = memory_map[free_block_index - 1].start_address;
        memory_map[free_block_index].segment_size = memory_map[free_block_index].segment_size + memory_map[free_block_index - 1].segment_size;
        for (int i = free_block_index - 1; i < *map_cnt; i++)
        {
            memory_map[i] = memory_map[i+1];
        }
        *map_cnt -= 1;
    }
}

