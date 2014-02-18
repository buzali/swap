//
//  process.h
//  Swap
//
//  Project 1 -- COMP30017
//  Tofi Buzali
//  tbuzali
//  
//
//  Created by Tofi Buzali on 25/03/11.
//
//  Main process struct contains info about the process
//

typedef struct
{
    int id;
    int size;
    int time_left;
}process;

void print_proccess (process *p);

