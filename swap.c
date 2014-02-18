//
//  swap.c
//  Swap
//
//  Project 1 -- COMP30017
//  Tofi Buzali
//  tbuzali
//  
//
//  Created by Tofi Buzali on 23/03/11.
//
//

#include "swap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "queue.h"
#include "free_list.h"


int current_time = 0;



void swap_in (struct queue *mQ, free_list *free_list, process *p,  int address)
//Swaps process p, taking care of the free list and mem list
{
    
    fill_address(free_list, address, p->size);

    enqueue(mQ, p, address, current_time);
}





void swap_out (queue *mQ, queue *dQ, free_list *fl, int p_id)
//Removes p_id from memory, frees address.
//Deletes process if finished, otherwise it goes back to disk
{
    int add;
    process *p;
    add = del_from_queue(mQ, p_id, &p);
    if (add == -1){
        printf("ERROR: process %d not found\n", p_id);
        return;
    }
    free_address(fl, add, p->size);
    
    // if time_left > 0, else delete
    if (p->time_left == 0){
        //process finished, free memory
        free(p);
    }
    else{
        //process has not finished, return it to disk
        enqueue(dQ, p, -1, current_time);
    }
}

void swap_out_next (queue *mQ, queue *dQ, free_list *fl)
//Swaps out the process that has been in memory the longest
{
    if (mQ->first)
        swap_out(mQ, dQ, fl, mQ->first->process->id);
}


process *swap (queue *mQ, queue *dQ, free_list *free_list,
               int mem_size, enum algorithm algo){
    //Swaps in a process from disk to memory using the specified algorithm.
    //Returns process swapped, otherwise returns NULL    
    
    process *to_b_swaped=NULL;
    
    
    //check if there's more than one process with same amount of time in disk
    struct Q_node *n;
    n = dQ->first;
    int smallest_id = -1;
    int smallest_toa = current_time; //smallest time of arrival (toa)
    
    //goes through the disk list to find p with smallest TOA
    while (n){
        if (n->TOA <= smallest_toa){
            to_b_swaped = n->process;
            smallest_toa = n->TOA;
        }
        n = n->next;
    }
    n = dQ->first;
    
    int i = 0; //number of processes with same TOA
    while ( n && n->TOA == smallest_toa){
        if (n->process->id < smallest_id || smallest_id ==-1)
            smallest_id = n->process->id;
        i++;
        n = n->next;
    }
    
    //If there's more than one process with the same TOA,
    //get the one with lowest priority.
    //otherwise take the next out of the queue
    if (i>1){
        del_from_queue(dQ, smallest_id, &to_b_swaped);
    }else if (to_b_swaped)
        del_from_queue(dQ, to_b_swaped->id, &to_b_swaped);
    
    if (to_b_swaped){        
        free_node *c_node = NULL; //node where process will be swaped into
        int best_size=mem_size+1;
        int largest_size=0;
        
        do{
            c_node = free_list->first; //node where process is to be swapped
            best_size=mem_size+1;
            largest_size=0;
            
            switch(algo){
                case FF:
                    while (c_node && c_node->size < to_b_swaped->size  )
                    {
                        c_node = c_node->next;
                    }
                    break;
                    
                case BF:
                    //Since there's no way to go backwards,
                    //and get the first smallest one.
                    //Goes through the list twice
                    
                    //determines best size
                    while (c_node){
                        if ((c_node->size < best_size) &&
                            (c_node->size >= to_b_swaped->size)){
                            best_size = c_node->size;
                            if (c_node->size == to_b_swaped->size)
                                break;
                        }
                        c_node = c_node->next;
                    }
                    c_node = free_list->first;
                    
                    //finds the node with size of best_size
                    while (c_node && c_node->size != best_size)
                        c_node = c_node->next;
                    break;
                    
                case WF:
                    
                    //finds largest hole size available
                    while (c_node){
                        if ((c_node->size > largest_size) &&
                            (c_node->size >= to_b_swaped->size)){
                            largest_size = c_node->size;
                            if (c_node->size == to_b_swaped->size)
                                break;
                        }
                        c_node = c_node->next;
                    }
                    c_node = free_list->first;
                    
                    //finds the node with that size
                    while (c_node && c_node->size != largest_size)
                        c_node = c_node->next;
                    break;
                default:
                    printf("ERROR: the algorithm %d does not exist\n", algo);
                    c_node = NULL;
                    break;
                    
            }
            
            
            //If there's no space for p, removes processes from memory
            if (!c_node){
                if (queue_empty_p(mQ)){
                    printf("ERROR: Not enough memory for process %d\n",
                           to_b_swaped->id);
                    return NULL;
                }
                //Not enough space, free oldest process in mem
                swap_out_next(mQ, dQ, free_list);
            }
        }
        //Repeats until there's enough space for the process
        while (!c_node); 
        
        if (c_node){
            //swaps in process to_b_swapped 
            swap_in(mQ, free_list, to_b_swaped, c_node->address);
            printf("time %d, %d loaded, numprocesses=%d, numholes=%d, "
                   "memusage=%d%%\n", current_time, to_b_swaped->id,
                   count_Q(mQ), count_list(free_list),
                   mem_usage(mQ, mem_size));
            return to_b_swaped;
        }else
            return NULL;
    }
    return NULL;
}

int process_file ( queue *fQ, char *filepath)
//Opens file, creates processes and inserts them into fQ
{
    FILE *file= fopen(filepath, "r");
    if (!file){
        printf("ERROR reading file %s\n", filepath);
        exit(1);
    }
    char lin[30];
    process *pa;
    int a_time = 999;
    while(fgets(lin, 100, file)){
        pa = malloc(sizeof(process));
        sscanf(lin, "%d %d %d %d", &a_time, &pa->id,
               &pa->size, &pa->time_left);
        enqueue(fQ, pa, -1, current_time);
        fQ->last->TOA = a_time;
    }
    fclose(file);
    return 0;
}



process *schedule(queue *mQ, int mem_size, int quatum, process *just_swapped){
    //Takes care of the round robin queue and the current_time
    //just_swapped should be the processes returned by swap
    //Returns proceess
    
    static queue rrQ; //Round robin queue
    static process *last; //Last proceess with CPU time
    static int initiated = 0;
    process *to_terminate;
    to_terminate = NULL;
    
    //Initializes queue the first time
    if (!initiated){
        init_queue(&rrQ);
        initiated = 1;
    }
    
    //Removes the processes from rr queue that are not in memory anymore 
    struct Q_node *n_rrQ;
    n_rrQ = rrQ.first;
    process *ret = NULL;
    int found = 0;
    
    int toDelete[mem_size]; //array of ids to delete
    int a_index =0;
    
    //If process is not in memory queue adds it to toDelete
    while(n_rrQ){
        found = find_in_queue(mQ, n_rrQ->process->id);
        if (!found){
            toDelete[a_index] = n_rrQ->process->id;
            a_index++;
        }
        n_rrQ = n_rrQ->next;
    }
    int i;
    
    //Deletes all the processes in toDelete
    for(i=0;i<a_index;i++)
    {
        del_from_queue(&rrQ, toDelete[i], &ret);
        ret = NULL;
    }
    
    
    if (just_swapped)
        enqueue(&rrQ, just_swapped, 0, current_time);
    
    //checks if the last process is still in mem
    if (last && find_in_queue(mQ, last->id))
        enqueue(&rrQ, last, 0, current_time);
    
    if (!queue_empty_p(&rrQ)){
        process *running;
        dequeue(&rrQ, &running);
        
        //calculates the actual quantum time since it may be smaller
        //if process finishes first
        int actual_quatum;
        if (running->time_left < quatum){
            actual_quatum = running->time_left;
        }else
            actual_quatum = quatum;
        
        running->time_left -= actual_quatum;
        current_time += actual_quatum;
        
        //If finished processing, return process that process
        if (running->time_left == 0){
            to_terminate = running;
            last = NULL;
        }else{
            last = running;
        }
        return to_terminate;
    }
    current_time++;
    return NULL;
}

void move_from_process_file (queue *dQ, queue *fQ)
//Takes care of adding the processes from the process file to the disk queue,
// taking the current time into account
{
    process *p=NULL;
    int tmp_TOA; //stores the time of arrival specified in process file
    while (!queue_empty_p(fQ) && fQ->first->TOA <=current_time){
        tmp_TOA = fQ->first->TOA;
        dequeue(fQ, &p);
        enqueue(dQ, p, -1, tmp_TOA);
    }
}


void swap_and_schedule ( int mem_size, int quantum, char *file_path,
                        enum algorithm algo )
//Main function. Takes care of the whole swap and schedule process
//Creates queues, loops through swap and schedule process until all processes
// are finished
{
    queue dQ;
    queue mQ;
    queue fQ; //stores process file
    free_list free_list;
    current_time = 0;
    
    //queue initialization
    init_queue(&dQ);
    init_queue(&mQ);
    init_queue(&fQ);
    init_free_list(&free_list, mem_size);
    
    //Read process file
    process_file(&fQ, file_path);

    process *current;
    process *to_terminate=NULL; //process finished 
    int i=0;
    do{
        move_from_process_file(&dQ, &fQ);
        current = swap(&mQ, &dQ, &free_list,mem_size, algo);
        to_terminate = schedule(&mQ, mem_size, quantum, current);
        //If process terminated, takes it out of memory.
        if (to_terminate){
            swap_out(&mQ, &dQ, &free_list, to_terminate->id);
            to_terminate = NULL;
        }
        i++;
    }while( !queue_empty_p(&mQ) || !queue_empty_p(&fQ) || !queue_empty_p(&dQ));
    
    printf("time %d, simulation finished.\n", current_time);

}

