#ifndef __PCB_INCLUDED__
#define __PCB_INCLUDED__

#include <stdio.h>
#include <sys/types.h>

typedef struct PCB PCB;

extern PCB *newPCB(int, int, int);
extern int getPCBarrivalTime(PCB *);
extern void setPCBarrivalTime(PCB *, int);
extern int getPCBpriority(PCB *);
extern void setPCBpriority(PCB *, int);
extern int getPCBprocessorTime(PCB *);
extern void setPCBprocessorTime(PCB *, int);
extern pid_t getPCBpid(PCB *);
extern void setPCBpid(PCB *, pid_t);
extern void displayPCB(PCB *, FILE *);
extern void freePCB(PCB *);

#endif // !__PCB_INCLUDED__
