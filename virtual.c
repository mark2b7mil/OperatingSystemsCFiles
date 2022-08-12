#include <stdio.h>
#include "oslabs.h"

int process_page_access_fifo(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp); 
int count_page_faults_fifo(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt);
int process_page_access_lru(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp); 
int count_page_faults_lru(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt);
int process_page_access_lfu(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp); 
int count_page_faults_lfu(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt);


int process_page_access_fifo(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp)
{
    if (page_table[page_number].is_valid == 1)
    {
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count += 1;
        return page_table[page_number].frame_number;
    }
    else
    {
        if (*frame_cnt > 0)
        {
            page_table[page_number].frame_number = frame_pool[*frame_cnt - 1];
            page_table[page_number].is_valid = 1;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;
            *frame_cnt -= 1;
            return page_table[page_number].frame_number;
        }
        else
        {
            int smallest_timestamp_index;
            int first_val = 0;
            for (int i = 0; i < *table_cnt; i++)
            {
                if (page_table[i].is_valid == 1 && first_val == 0)
                {
                    smallest_timestamp_index = i;
                    first_val += 1;
                }
                else if (page_table[i].is_valid == 1 && page_table[i].arrival_timestamp < page_table[smallest_timestamp_index].arrival_timestamp )
                {
                    smallest_timestamp_index = i;
                }
            }
            int frame_num = page_table[smallest_timestamp_index].frame_number;

            page_table[smallest_timestamp_index].is_valid = 0;
            page_table[smallest_timestamp_index].arrival_timestamp = -1;
            page_table[smallest_timestamp_index].frame_number = -1;
            page_table[smallest_timestamp_index].last_access_timestamp = -1;
            page_table[smallest_timestamp_index].reference_count = -1;

            page_table[page_number].frame_number = frame_num;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[page_number].is_valid = 1;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;
            return page_table[page_number].frame_number;
        }
    }
}

int count_page_faults_fifo(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt)
{
    int page_fault = 0;
    int timestamp = 0;
    for (int i = 0; i < table_cnt; i++)
    {
        if (page_table[i].is_valid == 1)
        {
            timestamp += 1;
        }
    }
    for (int k = 0; k < reference_cnt; k++)
    {
        timestamp += 1;
        if (page_table[refrence_string[k]].is_valid == 1)
        {
            page_table[refrence_string[k]].last_access_timestamp = timestamp;
            page_table[refrence_string[k]].reference_count += 1;
        }
        else
        {
            //page_fault += 1;
            if (frame_cnt > 0)
            {
                page_table[refrence_string[k]].frame_number = frame_pool[frame_cnt - 1];
                page_table[refrence_string[k]].is_valid = 1;
                page_table[refrence_string[k]].arrival_timestamp = timestamp;
                page_table[refrence_string[k]].last_access_timestamp = timestamp;
                page_table[refrence_string[k]].reference_count = 1;
                frame_cnt -= 1;
                page_fault += 1;
            }
            else
            {
                int smallest_timestamp_index;
                int first_val = 0;
                for (int i = 0; i < table_cnt; i++)
                {
                    if (page_table[i].is_valid == 1 && first_val == 0)
                    {
                        smallest_timestamp_index = i;
                        first_val += 1;
                    }
                    else if (page_table[i].is_valid == 1 && page_table[i].arrival_timestamp < page_table[smallest_timestamp_index].arrival_timestamp)
                    {
                        smallest_timestamp_index = i;
                    }
                }
                int frame_num = page_table[smallest_timestamp_index].frame_number;

                page_table[smallest_timestamp_index].is_valid = 0;
                page_table[smallest_timestamp_index].arrival_timestamp = 0;
                //page_table[smallest_timestamp_index].frame_number = 0;
                page_table[smallest_timestamp_index].last_access_timestamp = 0;
                page_table[smallest_timestamp_index].reference_count = 0;

                page_table[refrence_string[k]].frame_number = frame_num;
                page_table[refrence_string[k]].arrival_timestamp = timestamp;
                page_table[refrence_string[k]].is_valid = 1;
                page_table[refrence_string[k]].last_access_timestamp = timestamp;
                page_table[refrence_string[k]].reference_count = 1;
                page_fault += 1;
            }
        }
    }
    return page_fault;
}

int process_page_access_lru(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp)
{
    if (page_table[page_number].is_valid == 1)
    {
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count += 1;
        return page_table[page_number].frame_number;
    }
    else
    {
        if (*frame_cnt > 0)
        {
            page_table[page_number].frame_number = frame_pool[*frame_cnt - 1];
            page_table[page_number].is_valid = 1;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;
            *frame_cnt -= 1;
            return page_table[page_number].frame_number;
        }
        else
        {
            int special_index;
            int first_val = 0;
            for (int i = 0; i < *table_cnt; i++)
            {
                if (page_table[i].is_valid == 1 && first_val == 0)
                {
                    special_index = i;
                    first_val += 1;
                }
                else if (page_table[i].is_valid == 1 && page_table[i].last_access_timestamp < page_table[special_index].last_access_timestamp)
                {
                    special_index = i;
                }
            }
            int frame_num = page_table[special_index].frame_number;

            page_table[special_index].is_valid = 0;
            page_table[special_index].arrival_timestamp = -1;
            page_table[special_index].frame_number = -1;
            page_table[special_index].last_access_timestamp = -1;
            page_table[special_index].reference_count = -1;

            page_table[page_number].frame_number = frame_num;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[page_number].is_valid = 1;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;
            return page_table[page_number].frame_number;
        }
    }
}
int count_page_faults_lru(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt)
{
    int page_fault = 0;
    int timestamp = 0;
    for (int i = 0; i < table_cnt; i++)
    {
        if (page_table[i].is_valid == 1)
        {
            timestamp += 1;
        }
    }
    for (int k = 0; k < reference_cnt; k++)
    {
        timestamp += 1;
        if (page_table[refrence_string[k]].is_valid == 1)
        {
            page_table[refrence_string[k]].last_access_timestamp = timestamp;
            page_table[refrence_string[k]].reference_count += 1;
        }
        else
        {
            page_fault += 1;
            if (frame_cnt > 0)
            {
                page_table[refrence_string[k]].frame_number = frame_pool[frame_cnt - 1];
                page_table[refrence_string[k]].is_valid = 1;
                page_table[refrence_string[k]].arrival_timestamp = timestamp;
                page_table[refrence_string[k]].last_access_timestamp = timestamp;
                page_table[refrence_string[k]].reference_count = 1;
                frame_cnt -= 1;
                //page_fault += 1;
            }
            else
            {
                int special_index;
                int first_val = 0;
                for (int i = 0; i < table_cnt; i++)
                {
                    if (page_table[i].is_valid == 1 && first_val == 0)
                    {
                        special_index = i;
                        first_val += 1;
                    }
                    else if (page_table[i].is_valid == 1 && page_table[i].last_access_timestamp < page_table[special_index].last_access_timestamp)
                    {
                        special_index = i;
                    }
                }
                int frame_num = page_table[special_index].frame_number;

                page_table[special_index].is_valid = 0;
                page_table[special_index].arrival_timestamp = 0;
                //page_table[special_index].frame_number = 0;
                page_table[special_index].last_access_timestamp = 0;
                page_table[special_index].reference_count = 0;

                page_table[refrence_string[k]].frame_number = frame_num;
                page_table[refrence_string[k]].arrival_timestamp = timestamp;
                page_table[refrence_string[k]].is_valid = 1;
                page_table[refrence_string[k]].last_access_timestamp = timestamp;
                page_table[refrence_string[k]].reference_count = 1;
                //page_fault += 1;
            }
        }
    }
    return page_fault;
}
int process_page_access_lfu(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp)
{
    if (page_table[page_number].is_valid == 1)
    {
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count += 1;
        return page_table[page_number].frame_number;
    }
    else
    {
        if (*frame_cnt > 0)
        {
            page_table[page_number].frame_number = frame_pool[*frame_cnt - 1];
            page_table[page_number].is_valid = 1;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;
            *frame_cnt -= 1;
            return page_table[page_number].frame_number;
        }
        else
        {
            int special_index;
            int first_val = 0;
            for (int i = 0; i < *table_cnt; i++)
            {
                if (page_table[i].is_valid == 1 && first_val == 0)
                {
                    special_index = i;
                    first_val += 1;
                }
                else if (page_table[i].is_valid == 1 && page_table[i].reference_count < page_table[special_index].reference_count)
                {
                    special_index = i;
                }
                else if (page_table[i].is_valid == 1 && page_table[i].reference_count == page_table[special_index].reference_count && page_table[i].arrival_timestamp < page_table[special_index].arrival_timestamp)
                {
                    special_index = i;
                }
            }
            int frame_num = page_table[special_index].frame_number;

            page_table[special_index].is_valid = 0;
            page_table[special_index].arrival_timestamp = -1;
            page_table[special_index].frame_number = -1;
            page_table[special_index].last_access_timestamp = -1;
            page_table[special_index].reference_count = -1;

            page_table[page_number].frame_number = frame_num;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[page_number].is_valid = 1;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;
            return page_table[page_number].frame_number;
        }
    }
} 
int count_page_faults_lfu(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt)
{
    int page_fault = 0;
    int timestamp = 0;
    for (int k = 0; k < reference_cnt; k++)
    {
        timestamp += 1;
        if (page_table[refrence_string[k]].is_valid == 1)
        {
            page_table[refrence_string[k]].last_access_timestamp = timestamp;
            page_table[refrence_string[k]].reference_count += 1;
        }
        else
        {
            page_fault += 1;
            if (frame_cnt > 0)
            {
                page_table[refrence_string[k]].frame_number = frame_pool[frame_cnt - 1];
                page_table[refrence_string[k]].is_valid = 1;
                page_table[refrence_string[k]].arrival_timestamp = timestamp;
                page_table[refrence_string[k]].last_access_timestamp = timestamp;
                page_table[refrence_string[k]].reference_count = 1;
                frame_cnt -= 1;
                //page_fault += 1;
            }
            else
            {
                int special_index;
                int first_val = 0;
                for (int i = 0; i < table_cnt; i++)
                {
                    if (page_table[i].is_valid == 1 && first_val == 0)
                    {
                        special_index = i;
                        first_val += 1;
                    }
                    else if (page_table[i].is_valid == 1 && page_table[i].reference_count < page_table[special_index].reference_count)
                    {
                        special_index = i;
                    }
                    else if (page_table[i].is_valid == 1 && page_table[i].reference_count == page_table[special_index].reference_count && page_table[i].arrival_timestamp < page_table[special_index].arrival_timestamp)
                    {
                        special_index = i;
                    }
                }
                int frame_num = page_table[special_index].frame_number;

                page_table[special_index].is_valid = 0;
                page_table[special_index].arrival_timestamp = 0;
                //page_table[special_index].frame_number = 0;
                page_table[special_index].last_access_timestamp = 0;
                page_table[special_index].reference_count = 0;

                page_table[refrence_string[k]].frame_number = frame_num;
                page_table[refrence_string[k]].arrival_timestamp = timestamp;
                page_table[refrence_string[k]].is_valid = 1;
                page_table[refrence_string[k]].last_access_timestamp = timestamp;
                page_table[refrence_string[k]].reference_count = 1;
                //page_fault += 1;
            }
        }
    }
    return page_fault;
}