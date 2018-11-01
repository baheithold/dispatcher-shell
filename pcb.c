#include "pcb.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

struct PCB {
    pid_t pid;
    int arrivalTime;
    int priority;
    int processorTime;
};

PCB *newPCB(int arrival, int priority, int processor) {
    assert(arrival >= 0);
    assert(priority >= 0);
    assert(processor >= 0);
    PCB *p = malloc(sizeof(PCB));
    assert(p != 0);
    p->pid = -1;
    p->arrivalTime = arrival;
    p->priority = priority;
    p->processorTime = processor;
    return p;
}

void displayPCB(PCB *p, FILE *fp) {
    fprintf(fp, "pid: %d arrival time: %d priority: %d processor time: %d\n", p->pid, p->arrivalTime, p->priority, p->processorTime);
}
