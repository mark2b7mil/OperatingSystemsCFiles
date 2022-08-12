#include <stdio.h>
#include "oslabs.h"

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp);
struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp);
struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp);
struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp);
struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum);
struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int time_stamp, int time_quantum);

struct PCB NULLPCB = {0,0,0,0,0,0,0};

void remove_element(struct PCB ready_queue[QUEUEMAX], int index, int array_length)
{
   int i;
   for(i = index; i < array_length - 1; i++) ready_queue[i] = ready_queue[i + 1];
}

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp)
{
    if (current_process.process_id == NULLPCB.process_id && current_process.arrival_timestamp == NULLPCB.arrival_timestamp)
    {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
    else
    {
        if (new_process.process_priority > current_process.process_priority)
        {
            new_process.execution_starttime = 0;
            new_process.execution_endtime = 0;
            new_process.remaining_bursttime = new_process.total_bursttime;
            ready_queue[*queue_cnt] = new_process;
            *queue_cnt += 1;
            return current_process;
        }
        else
        {
            current_process.execution_endtime = 0;
            current_process.remaining_bursttime = current_process.total_bursttime - (timestamp - current_process.arrival_timestamp);
            ready_queue[*queue_cnt] = current_process;
            *queue_cnt += 1;
            new_process.execution_starttime = timestamp;
            new_process.execution_endtime = timestamp + new_process.total_bursttime;
            new_process.remaining_bursttime = new_process.total_bursttime;
            return new_process;
        }
    }
    
}

struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp)
{
    if (*queue_cnt == 0)
    {
        return NULLPCB;
    }
    else
    {
        struct PCB highest_priority = ready_queue[0];
        int max_index = 0;
        for (int i = 0; i < *queue_cnt; i++)
        {
            if (ready_queue[i].process_priority < highest_priority.process_priority)
            {
                highest_priority = ready_queue[i];
                max_index = i;
            }
        }
        highest_priority.execution_starttime = timestamp;
        highest_priority.execution_endtime = timestamp + highest_priority.remaining_bursttime;
        remove_element(ready_queue, max_index, *queue_cnt);
        *queue_cnt -= 1;
        return highest_priority;
    }
}

struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp)
{
    if (current_process.process_id == NULLPCB.process_id && current_process.arrival_timestamp == NULLPCB.arrival_timestamp)
    {
        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
    else
    {
        if(new_process.total_bursttime >= current_process.remaining_bursttime)
        {
            new_process.remaining_bursttime = new_process.total_bursttime;
            new_process.execution_starttime = 0;
            new_process.execution_endtime = 0;
            ready_queue[*queue_cnt] = new_process;
            *queue_cnt += 1;
            return current_process;
        }
        else
        {
            current_process.execution_endtime = 0;
            current_process.execution_starttime = 0;
            current_process.remaining_bursttime = current_process.total_bursttime - (time_stamp - current_process.arrival_timestamp);
            ready_queue[*queue_cnt] = current_process;
            *queue_cnt += 1;
            new_process.execution_starttime = time_stamp;
            new_process.execution_endtime = time_stamp + new_process.total_bursttime;
            new_process.remaining_bursttime = new_process.total_bursttime;
            return new_process;
        }
    }
}

struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp)
{
    if (*queue_cnt == 0)
    {
        return NULLPCB;
    }
    else
    {
        struct PCB smallest_burst = ready_queue[0];
        int max_index = 0;
        for (int i = 0; i < *queue_cnt; i++)
        {
            if (ready_queue[i].remaining_bursttime < smallest_burst.remaining_bursttime)
            {
                smallest_burst = ready_queue[i];
                max_index = i;
            }
        }
        smallest_burst.execution_starttime = timestamp;
        smallest_burst.execution_endtime = timestamp + smallest_burst.remaining_bursttime;
        remove_element(ready_queue, max_index, *queue_cnt);
        *queue_cnt -= 1;
        return smallest_burst;
    }
}

struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum)
{
    if (current_process.process_id == NULLPCB.process_id && current_process.arrival_timestamp == NULLPCB.arrival_timestamp)
    {
        int smallest_time;
        if (time_quantum <= new_process.remaining_bursttime)
        {
            smallest_time = time_quantum;
        }
        else
        {
            smallest_time = new_process.remaining_bursttime;
        }
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + smallest_time;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
    else
    {
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        ready_queue[*queue_cnt] = new_process;
        *queue_cnt += 1;
        return current_process;
    }
}

struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int time_stamp, int time_quantum)
{
    if (*queue_cnt == 0)
    {
        return NULLPCB;
    }
    else
    {
        struct PCB earliest_arrival = ready_queue[0];
        int max_index = 0;
        for (int i = 0; i < *queue_cnt; i++)
        {
            if (ready_queue[i].arrival_timestamp < earliest_arrival.arrival_timestamp)
            {
                earliest_arrival = ready_queue[i];
                max_index = i;
            }
        }
        int smallest_time;
        if (time_quantum <= earliest_arrival.remaining_bursttime)
        {
            smallest_time = time_quantum;
        }
        else
        {
            smallest_time = earliest_arrival.remaining_bursttime;
        }
        earliest_arrival.execution_starttime = time_stamp;
        earliest_arrival.execution_endtime = time_stamp + smallest_time;
        remove_element(ready_queue, max_index, *queue_cnt);
        *queue_cnt -= 1;
        return earliest_arrival;
    }
}

