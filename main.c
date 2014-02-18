//
//  main.c
//  Swap
//
//  Project 1 -- COMP30017
//  Tofi Buzali
//  tbuzali
//  
//
//  Created by Tofi Buzali on 31/03/11.
//
//


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "swap.h"


extern  int     optind;
extern  char    *optarg;

int main (int argc, char * argv[])
//Calls swap_and_schedule function using parameters received from cmd line
{

    char input;
    int quantum =-1;
    enum algorithm algo = -1;
    char *file_path =NULL;
    int mem_size = -1;
    
    while ((input = getopt(argc, argv, "f:a:m:q:")) != EOF)
	{
		switch ( input )
		{
			case 'f':
                file_path = optarg;
 				break;
                
			case 'a':
                if(strcmp(optarg, "first") == 0) 
					algo  = FF;
                else if(strcmp(optarg, "best") == 0) 
					algo  = BF;
                else if(strcmp(optarg, "worst") == 0) 
                    algo = WF;
				else{
					fprintf(stderr, "Unknown algorithm %s\n ", optarg);
				}
 				break;
                
            case 'm':
                mem_size = atoi(optarg);
                if (mem_size <1){
                    fprintf(stderr, "Memory is too small, gimme some more\n");
                    mem_size = -1;
                }
                break;
                
            case 'q':
                quantum = atoi(optarg);
                if (quantum <1){
                    fprintf(stderr, "Quantum is too small,"
                           "gimme something different\n");
                    quantum = -1;
                }
                break;
            
            default:
				break;
		} 
	}
    
    //At least one parameter is wrong. Prints Usage info
    if (quantum == -1 || algo == -1 || file_path == NULL || mem_size ==  -1){
        fprintf(stderr, "Usage: swap [options]\n"
               "Required parameters:\n\n"
        "-f <file>\n\n"
        "-a <algorithm>\tfirst, best or worst\n\n"
        "-m <memory size>\t\t\t\n\n"
        "-q <quantum size>\n\n");
        exit(-1);
    }

    swap_and_schedule (mem_size, quantum, file_path, algo);

    return 0;
}
