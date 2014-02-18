//
//  free_list.h
//  Swap
//
//  Project 1 -- COMP30017
//  Tofi Buzali
//  tbuzali
//  
//
//  Created by Tofi Buzali on 25/03/11.
//
//  Linked list to store holes of free memory 
//

struct free_node{
    int address;
    int size;
    struct free_node *next;
};
typedef struct free_node free_node;

typedef struct {
    struct free_node *first;
}free_list;


void print_list(free_list *fl);
int count_list(free_list *fl);
void init_free_list(free_list *fl, int size);
void free_address(free_list *fl, int add, int size);
void fill_address (free_list *free_list,  int address, int size);
