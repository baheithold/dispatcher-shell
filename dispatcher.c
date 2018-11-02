#include "pcb.h"
#include "queue.h"
#include "scanner.h"
#include "sll.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

SLL *createDispatcherList(char *filename);

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <filepath>\n", argv[0]);
        exit(1);
    }

    // Create Dispatcher List
    // Arrival Times are ordered based on arrival time earliest to latest
    SLL *dispatcherList = createDispatcherList(argv[1]);

    return 0;
}

SLL *createDispatcherList(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == 0) {
        printf("Error: Cannont open %s for reading!\n", filename);
        exit(1);
    }
    SLL *dispatcherList = newSLL(displayPCB, freePCB);
    int arrival, priority, processor;
    char trash; // for commas
    arrival = readInt(fp);
    while (!feof(fp)) {
        trash = readChar(fp);
        priority = readInt(fp);
        trash = readChar(fp);
        processor = readInt(fp);
        PCB *pcb = newPCB(arrival, priority, processor);
        insertSLL(dispatcherList, sizeSLL(dispatcherList), pcb);
        arrival = readInt(fp);
    }
    (void) trash; // trick compiler for unused variable warning
    return dispatcherList;
}
