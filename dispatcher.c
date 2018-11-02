#include "pcb.h"
#include "queue.h"
#include "scanner.h"
#include "sll.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define TIME_QUANTUM    1
#define SYSTEM_PRIORITY 0
#define HIGH_PRIORITY   1
#define MEDIUM_PRIORITY 2
#define LOW_PRIORITY    3


/* Function Prototypes */
SLL *createDispatcherList(char *);
int processesRemain(SLL *);
void getArrivedProcesses(int, SLL *, QUEUE *, QUEUE *, QUEUE *, QUEUE *);


int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <filepath>\n", argv[0]);
        exit(1);
    }

    // Create Dispatcher List
    // Arrival Times are ordered based on arrival time earliest to latest
    // Arrival times in list: earliest -> latest = head -> tail
    SLL *dispatcherList = createDispatcherList(argv[1]);

    // Create Process Queues
    QUEUE *systemQueue = newQUEUE(displayPCB, freePCB);
    QUEUE *highQueue = newQUEUE(displayPCB, freePCB);
    QUEUE *mediumQueue = newQUEUE(displayPCB, freePCB);
    QUEUE *lowQueue = newQUEUE(displayPCB, freePCB);

    int current_time = 0;
    getArrivedProcesses(current_time, dispatcherList, systemQueue, highQueue, mediumQueue, lowQueue);

    freeSLL(dispatcherList);
    freeQUEUE(systemQueue);
    freeQUEUE(highQueue);
    freeQUEUE(mediumQueue);
    freeQUEUE(lowQueue);
    return 0;
}


/* Function Definitions */

SLL *createDispatcherList(char *filename) {
    // Arrival Times are ordered based on arrival time earliest to latest
    // Arrival times in list: earliest -> latest = head -> tail
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
    fclose(fp);
    return dispatcherList;
}

int processesRemain(SLL *dispatcherList) {
    assert(dispatcherList != 0);
    return sizeSLL(dispatcherList);
}

void getArrivedProcesses(int current_time, SLL *dispatcherList, QUEUE *system, QUEUE *highQueue, QUEUE *mediumQueue, QUEUE *lowQueue) {
    assert(current_time >= 0);
    assert(dispatcherList != 0);
    assert(system != 0);
    assert(highQueue != 0);
    assert(mediumQueue != 0);
    assert(lowQueue != 0);
    while (processesRemain(dispatcherList) && getPCBarrivalTime(getSLL(dispatcherList, 0)) <= current_time) {
        PCB *pcb = removeSLL(dispatcherList, 0);
        switch (getPCBpriority(pcb)) {
            case SYSTEM_PRIORITY:
                enqueue(system, pcb);
                break;
            case HIGH_PRIORITY:
                enqueue(highQueue, pcb);
                break;
            case MEDIUM_PRIORITY:
                enqueue(mediumQueue, pcb);
                break;
            case LOW_PRIORITY:
                enqueue(lowQueue, pcb);
                break;
            default:
                break;
        }
    }
}
