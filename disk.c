#include <stdio.h>
#include <stdlib.h>
#include "oslabs.h"

struct RCB NULLRCB = {0,0,0,0,0};

struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp); 
struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX],int *queue_cnt); 
struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp); 
struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt,int current_cylinder); 
struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp);
struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX],int  *queue_cnt, int current_cylinder, int scan_direction); 


struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp)
{
    if (current_request.address == 0 && current_request.arrival_timestamp == 0 && current_request.cylinder == 0 && current_request.process_id == 0 && current_request.request_id == 0)
    {
        return new_request;
    }
    else
    {
        request_queue[*queue_cnt] = new_request;
        *queue_cnt += 1;
        return current_request;
    }
}

struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX],int *queue_cnt)
{
    if (*queue_cnt == 0)
    {
        return NULLRCB;
    }
    else
    {
        int index = 0;
        for (int i = 0; i < *queue_cnt; i++)
        {
            if (request_queue[i].arrival_timestamp < request_queue[index].arrival_timestamp)
            {
                index = i;
            }
        }
        struct RCB return_request = request_queue[index];
        for (int k = index; k < *queue_cnt; k++)
        {
            request_queue[k] = request_queue[k+1];
        }
        *queue_cnt -= 1;
        return return_request;
    }
}

struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp)
{
    if (current_request.address == 0 && current_request.arrival_timestamp == 0 && current_request.cylinder == 0 && current_request.process_id == 0 && current_request.request_id == 0)
    {
        return new_request;
    }
    else
    {
        request_queue[*queue_cnt] = new_request;
        *queue_cnt += 1;
        return current_request;
    }
}

struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt,int current_cylinder)
{
    if (*queue_cnt == 0)
    {
        return NULLRCB;
    }
    else
    {
        int index = 0;
        for (int i = 0; i < *queue_cnt; i++)
        {
            if (abs(request_queue[i].cylinder - current_cylinder) <= abs(request_queue[index].cylinder - current_cylinder))
            {
                if (abs(request_queue[i].cylinder - current_cylinder) == abs(request_queue[index].cylinder - current_cylinder))
                {
                    if (request_queue[i].arrival_timestamp < request_queue[index].arrival_timestamp)
                    {
                        index = i;
                    }
                }
                else
                {
                    index = i;
                }
            }
        }
        struct RCB return_request = request_queue[index];
        for (int k = index; k < *queue_cnt; k++)
        {
            request_queue[k] = request_queue[k+1];
        }
        *queue_cnt -= 1;
        return return_request;
    }
}

struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp)
{
    if (current_request.address == 0 && current_request.arrival_timestamp == 0 && current_request.cylinder == 0 && current_request.process_id == 0 && current_request.request_id == 0)
    {
        return new_request;
    }
    else
    {
        new_request.arrival_timestamp = timestamp;
        request_queue[*queue_cnt] = new_request;
        *queue_cnt += 1;
        return current_request;
    }
}

struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX],int  *queue_cnt, int current_cylinder, int scan_direction)
{
    if (*queue_cnt == 0)
    {
        return NULLRCB;
    }
    else
    {
        int index = 0;
        int first = 0;
        int cylinder_flag = 0;
        int larger = 0;
        int smaller = 0;
        for (int i = 0; i < *queue_cnt; i++)
        {
            if (request_queue[i].cylinder == current_cylinder)
            {
                if (first == 0)
                {
                    cylinder_flag = 1;
                    index = i;
                    first = 1;    
                }
                else if (request_queue[i].arrival_timestamp < request_queue[index].arrival_timestamp)
                {
                    index = i;
                }
            }
            else if (request_queue[i].cylinder > current_cylinder)
            {
                larger = 1;
            }
            else if (request_queue[i].cylinder < current_cylinder)
            {
                smaller = 1;
            }
        }

        if (cylinder_flag == 0)
        {
            if (scan_direction == 1 && larger == 1)
            {
                first = 0;
                for (int k = 0; k < *queue_cnt; k++)
                {
                    if (request_queue[k].cylinder > current_cylinder && first == 0)
                    {
                        index = k;
                        first = 1;
                    }
                    else if (request_queue[k].cylinder > current_cylinder && abs(request_queue[k].cylinder - current_cylinder) < abs(request_queue[index].cylinder - current_cylinder) && first == 1)
                    {
                        index = k;
                    }
                }
                    
            }
            else if (scan_direction == 1 && larger == 0)
            {
                first = 0;
                for (int k = 0; k < *queue_cnt; k++)
                {
                    if (first == 0)
                    {
                        index = k;
                        first = 1;
                    }
                    else if (abs(request_queue[k].cylinder - current_cylinder) < abs(request_queue[index].cylinder - current_cylinder) && first == 1)
                    {
                        index = k;
                    }
                }
            }
            else if (scan_direction == 0 && smaller == 1)
            {
                first = 0;
                for (int k = 0; k < *queue_cnt; k++)
                {
                    if (request_queue[k].cylinder < current_cylinder && first == 0)
                    {
                        index = k;
                        first = 1;
                    }
                    else if (request_queue[k].cylinder < current_cylinder && abs(request_queue[k].cylinder - current_cylinder) < abs(request_queue[index].cylinder - current_cylinder) && first == 1)
                    {
                        index = k;
                    }
                }  
            }
            else if (scan_direction == 0 && smaller == 0)
            {
                first = 0;
                for (int k = 0; k < *queue_cnt; k++)
                {
                    if (first == 0)
                    {
                        index = k;
                        first = 1;
                    }
                    else if (abs(request_queue[k].cylinder - current_cylinder) < abs(request_queue[index].cylinder - current_cylinder) && first == 1)
                    {
                        index = k;
                    }
                }
            }
        }

        struct RCB return_request = request_queue[index];
        for (int j = index; j < *queue_cnt; j++)
        {
            request_queue[j] = request_queue[j+1];
        }
        *queue_cnt -= 1;
        return return_request;
    }
}