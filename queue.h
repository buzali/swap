//
//  queue.h
//  Swap
//
//  Project 1 -- COMP30017
//  Tofi Buzali
//  tbuzali
//  
//
//  Created by Tofi Buzali on 25/03/11.
//
//  Memory queue which stores processes, address and time of arrival
//

#include "process.h"

struct Q_node
{
    struct Q_node *next; 
    int TOA; //time when process arrived to queue
    int address;
    process *process;
};

struct queue
{
    struct Q_node *first;
    struct Q_node *last;
};
typedef struct queue queue;

int enqueue( queue *q, process *p, int add, int toa);
int dequeue(struct queue *q, process **return_p);
void init_queue(struct queue *q);
int del_from_queue(struct queue *q, int p_id, process **return_p);
int queue_ordered_q(queue *q);
int queue_empty_p(const struct queue *q);
int find_in_queue(queue *q, int process_id);
int count_Q (struct queue *q);
int mem_usage(struct queue *q, int mem_size);
void print_Q( struct queue *q);

