//
//  queue.c
//  Swap
//
//  Project 1 -- COMP30017
//  Tofi Buzali
//  tbuzali
//  
//
//  Created by Tofi Buzali on 25/03/11.
//
//
//  This code is based on the Queue C Example from Wikipedia:
//  Queue (data structure). (2011, April 2). In Wikipedia,
//  The Free Encyclopedia. Retrieved 08:58, April 4, 2011,
//  from http://en.wikipedia.org/w/index.php?title=
//  Queue_(data_structure)&oldid=422040796
//  

#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


int enqueue( queue *q, process *p, int add, int toa)
//Inserts process p into the queue
{
    struct Q_node *node = malloc(sizeof(struct Q_node));
    if (node == NULL) {
        return 1;
    }
    node->process = p;
    node->TOA = toa;
    node->address = add;
    if (q->first == NULL) {
        q->first = q->last = node;
    } else {
        q->last->next = node;
        q->last = node;
    }
    node->next = NULL;
    return 0;
}

int dequeue(struct queue *q, process **return_p)
//Removes first item in queue, returns its address
{
    if (!q->first) {
        *return_p = NULL;
        return -1;
    }
    process *p = q->first->process;
    struct Q_node *tmp = q->first;
    q->first = q->first->next;
    int add = tmp->address;
    free(tmp);
    *return_p = p;
    return add;
}

void init_queue(struct queue *q)
//Initializes queue
{
    q->first = q->last = NULL;
}

int queue_empty_p(const struct queue *q)
//Returns 1 if queue is empty, otherwise returns 0
{
    return q->first == NULL;
}

int del_from_queue(struct queue *q, int p_id, process **return_p)
//Deletes process with id p_id from the queue q, and returns it to *return_p
{
    struct Q_node *n;
    struct Q_node *previous = NULL;
    n = q->first;

    //Looks for the node and previous node of the process to be removed.
    while (n && n->process->id != p_id){
        previous = n;
        n = n->next;
    }
    if (!n){
        printf("Could not find process %d in queue\n", p_id);
        return -1;
    }
    if (previous && n->next)
        //process node is in the middle of two nodes
        previous->next = n->next;
    else if (n->next)
        //process node is the first in the queue
    {
        q->first = n->next;
    }
    else if (previous){
        //process node is at the end of the queue
        q->last = previous;
        previous->next = NULL;
    }
    else{
        //It's the last node in the queue... initialize queue
        init_queue(q);
    }
    int add = n->address;
    *return_p = n->process;
    free(n);
    return add;
}


void test_disk_queue(void)
//Tests queue, used for debugging
{
    struct queue Q;
    init_queue(&Q);
    process *el_p;
    
    unsigned int k = 0;
    while (k < 100) {
        el_p = malloc(sizeof(process));
        el_p->id=k;
        enqueue(&Q, el_p, -1, 0);
        printf("p_id: %d\n", el_p->id);
        ++k;
    }
    k = 1;
    while (!queue_empty_p(&Q)) {
        process *p;
        dequeue(&Q, &p);
        printf("\n");
        printf("(%d) %d\n", k, p->id);
        ++k;
    }
}

int queue_ordered_q(queue *q)
//Returns 1/0 if processes in the q are ordered by toa
{
    int last = -1;
    struct Q_node *n;
    n = q->first;
    while (n)
    {
        if (last > n->TOA)
            return 0;
        last = n->TOA;
        n = n->next;
    }
    return 1;
}

int find_in_queue(queue *q, int process_id)
//Returns 1/0 if process with process_id was/wasn't found in queue q
{
    struct Q_node *n;
    if (q)
        n = q->first;
    int found = 0;
    //Goes through all the nodes in the queue
    while(n){
        if (n->process->id == process_id){
            found = 1;
            break;
        }
        n = n->next;
    }
    return found;
}


int count_Q (struct queue *q)
//Returns number of items in the queue
{
    int count =0;
    struct Q_node *n;
    if (q->first){
        n = q->first;
        while (n){
            count++;
            n = n->next;
        }
    }
    return count;
}

int mem_usage(struct queue *q, int mem_size)
//Calculates memory usage using mem_size as the size of the entire memory
{
    struct Q_node *n;
    n = q->first;
    double used = 0;
    //Goes through the queue and adds memory usage
    while (n)
    {
        used += n->process->size;
        n = n->next;
    }
    double usage = (used/mem_size)*100;
    return ceil(usage);
}



void print_Q( struct queue *q)
//Prints queue q
{
    struct Q_node *n;
    n = q->first;
    if (n){
        printf("%d", n->process->id);
        n = n->next;
    }
    while (n){
        printf("<--%d", n->process->id);
        n = n->next;
    }
    printf("\n");
}


