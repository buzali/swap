//
//  process.c
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

#include "process.h"
#include <stdlib.h>
#include <stdio.h>

void print_proccess (process *p)
//Prints process p
{
    printf("p(%d):\tmem:%d\tt_left:%d\n",p->id, p->size, p->time_left );
    
}
