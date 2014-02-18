//
//  swap.h
//  Swap
//
//  Project 1 -- COMP30017
//  Tofi Buzali
//  tbuzali
//  
//
//  Created by Tofi Buzali on 31/03/11.
//
//  Memory management function using FF, BF, or WF algorithms
//


enum algorithm {
    FF, //First Fit
    BF, //Best Fit
    WF //Worst Fit
};

void swap_and_schedule (int mem_size, int quantum, char *file_path,
                        enum algorithm algo);

