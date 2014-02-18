//
//  free_list.c
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

#include "free_list.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>




void print_list(free_list *fl)
//prints free_list
{
    free_node *n;
    n = fl->first;
    if (n){
        printf("<%d-%d>", n->address, n->size);
        n = n->next;
    }
    while (n){
        printf("--<%d-%d>",n->address, n->size);
        n = n->next;
    }
    printf("\n");
}

int count_list(free_list *fl)
//Counts the number of holes
{
    free_node *n;
    n = fl->first;
    int count =0;
    while (n)
    {
        if (n->address !=0)
            count ++;
        n = n->next;
    }
    return count;
}


void init_free_list(free_list *fl, int size)
//Initializes free_list fl
{
    free_node *free_first;
    free_first = malloc(sizeof(free_node));
    free_first->address = size;
    free_first->size = size;
    free_first->next = NULL;
    fl->first = free_first;
}

void merge_free_list(free_list *fl)
//Merges adjacent holes
{
    free_node *n;
    free_node *tmp;
    n=fl->first;
    while(n){
    //If the address and size is equal to the next item in list,merge them.
        if (n->next){
            if ( (n->address - n->size) == n->next->address){
                n->size += n->next->size;
                tmp = n->next;
                n->next = tmp->next;
                free(tmp);
            }else if (n->next)
                n=n->next;
        }
        else
            return;
    }
}

void free_address(free_list *fl, int add, int size)
//Add a hole with address add and size
{
    
    free_node *new_node = malloc(sizeof(free_node));
    new_node->address = add;
    new_node->size = size;
    new_node->next = NULL;
    
    free_node *n;
    free_node *previous =NULL;
    free_node *tmp;
    n = fl->first;
    //Finds the next and previous items on the link list
    while(n && add <= (n->address - n->size)){
        previous = n;
        n = n->next;
    }
    if (previous){
        tmp = previous->next;
        previous->next = new_node;
        new_node->next = tmp;
    }else{
    //First hole in the list
        if (n)
            new_node->next = n;
        fl->first = new_node;
    }
    
    //Merge adjacent holes
    merge_free_list(fl);
    
}

void fill_address (free_list *free_list,  int address, int size)
//Removes hole from list
{
    
    free_node *out_node = NULL; //node to be removed
    free_node *previous = NULL;
    
    //Looks for node to be removed
    out_node = free_list->first;
    while ( out_node && out_node->address != address ){
        previous = out_node;
        out_node=out_node->next;
    }
    assert(out_node->size >= size);
    if (!out_node)
        printf("ERROR, no se encontro free node con add %d", address);
    
    //If node is bigger than size of process, shrink hole
    // otherwise remove node completely.
    if (out_node->size > size){
        out_node->address = out_node->address - size;
        out_node->size = out_node->size - size;
    }else{
        if (previous)
            previous->next= out_node->next;
        else
            free_list->first = out_node->next;
        free(out_node);
    }
}

