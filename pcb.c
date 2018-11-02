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

int getPCBarrivalTime(PCB *pcb) {
    assert(pcb != 0);
    return pcb->arrivalTime;
}

void setPCBarrivalTime(PCB *pcb, int time) {
    assert(pcb != 0);
    assert(time >= 0);
    pcb->arrivalTime = time;
}

int getPCBpriority(PCB *pcb) {
    assert(pcb != 0);
    return pcb->priority;
}

void setPCBpriority(PCB *pcb, int priority) {
    assert(pcb != 0);
    assert(priority >= 0);
    pcb->priority = priority;
}

int getPCBprocessorTime(PCB *pcb) {
    assert(pcb != 0);
    return pcb->processorTime;
}

void setPCBprocessorTime(PCB *pcb, int time) {
    assert(pcb != 0);
    assert(time >= 0);
    pcb->processorTime = time;
}

pid_t getPCBpid(PCB *pcb) {
    assert(pcb != 0);
    return pcb->pid;
}

void setPCBpid(PCB *pcb, pid_t pid) {
    assert(pcb != 0);
    pcb->pid = pid;
}

void displayPCB(void *pcb, FILE *fp) {
    assert(pcb != 0);
    fprintf(fp, "pid: %d arrival time: %d priority: %d processor time: %d\n", ((PCB *) pcb)->pid, ((PCB *) pcb)->arrivalTime, ((PCB *) pcb)->priority, ((PCB *) pcb)->processorTime);
}

void freePCB(void *pcb) {
    assert(pcb != 0);
    free((PCB *) pcb);
}
