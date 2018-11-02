#ifndef __PCB_INCLUDED__
#define __PCB_INCLUDED__

#include <stdio.h>
#include <sys/types.h>

typedef struct PCB PCB;

typedef enum {
    READY,
    WAITING
} pcb_state;

extern PCB *newPCB(int, int, int);
extern int getPCBarrivalTime(PCB *);
extern void setPCBarrivalTime(PCB *, int);
extern int getPCBpriority(PCB *);
extern void setPCBpriority(PCB *, int);
extern int getPCBprocessorTime(PCB *);
extern void decrementPCBprocessorTime(PCB *);
extern pid_t getPCBpid(PCB *);
extern void setPCBpid(PCB *, pid_t);
extern pcb_state getPCBstate(PCB *);
extern void setPCBstate(PCB *, pcb_state);
extern void displayPCB(void *, FILE *);
extern void freePCB(void *);

#endif // !__PCB_INCLUDED__
