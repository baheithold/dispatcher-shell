#define _POSIX_SOURCE
#include "pcb.h"
#include "queue.h"
#include "scanner.h"
#include "sll.h"
#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define TIME_QUANTUM    1
#define SYSTEM_PRIORITY 0
#define HIGH_PRIORITY   1
#define MEDIUM_PRIORITY 2
#define LOW_PRIORITY    3


/* Function Prototypes */
SLL *createDispatcherList(char *);
int processesRemain(SLL *);
void getArrivedProcesses(int, SLL *, QUEUE *, QUEUE *, QUEUE *, QUEUE *);
void enqueueBasedOnPriority(PCB *, QUEUE *, QUEUE *, QUEUE *, QUEUE *);
void lowerPCBpriority(PCB *);
void startProcess(PCB *process);
void terminateProcess(PCB *process);
void suspendProcess(PCB *process);
void restartProcess(PCB *process);


int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <filepath>\n", argv[0]);
        exit(1);
    }

    // Create Dispatcher List
    // Arrival Times are ordered based on arrival time earliest to latest
    // Arrival times in list: earliest -> latest = head -> tail
    SLL *dispatchList = createDispatcherList(argv[1]);

    // Create Process Queues
    QUEUE *systemQueue = newQUEUE(displayPCB, freePCB);
    QUEUE *highQueue = newQUEUE(displayPCB, freePCB);
    QUEUE *mediumQueue = newQUEUE(displayPCB, freePCB);
    QUEUE *lowQueue = newQUEUE(displayPCB, freePCB);

    // Run Processes
    int current_time = 0;
    int system_process_running = 0;
    PCB *current_process_running = 0;
    while (current_process_running || processesRemain(dispatchList)) {
        getArrivedProcesses(current_time, dispatchList, systemQueue, highQueue, mediumQueue, lowQueue);
        // check if currently running process needs terminating
        if (current_process_running && getPCBprocessorTime(current_process_running) == 0) {
            terminateProcess(current_process_running);
            if (system_process_running) {
                system_process_running = 0;
            }
            freePCB(current_process_running);
            current_process_running = 0;
        }
        // Check if system process needs to be run
        if (!system_process_running && sizeQUEUE(systemQueue) > 0) {
            if (current_process_running) {
                suspendProcess(current_process_running);
                if (getPCBpriority(current_process_running) < 3) {
                    lowerPCBpriority(current_process_running);
                }
                enqueueBasedOnPriority(current_process_running, systemQueue, highQueue, mediumQueue, lowQueue);
            }
            PCB *sysProcess = dequeue(systemQueue);
            startProcess(sysProcess);
            current_process_running = sysProcess;
            system_process_running = 1;
        }
        else if (system_process_running) {
            // Do nothing
        }
        // if there is no system processes running and there is a process waiting to be run
        else if (sizeQUEUE(highQueue) > 0) {
            // high priority process waiting to be run
            if (current_process_running) {
                suspendProcess(current_process_running);
                if (getPCBpriority(current_process_running) < 3) {
                    lowerPCBpriority(current_process_running);
                }
                enqueueBasedOnPriority(current_process_running, systemQueue, highQueue, mediumQueue, lowQueue);
            }
            PCB *highProcess = dequeue(highQueue);
            if (getPCBstate(highProcess) == READY) {
                // Process is ready to run
                startProcess(highProcess);
            }
            else {
                // Process has been previously suspended and must be restarted
                restartProcess(highProcess);
            }
            current_process_running = highProcess;
            system_process_running = 0;
        }
        else if (sizeQUEUE(mediumQueue) > 0) {
            // medium priority process waiting to be run
            if (current_process_running) {
                suspendProcess(current_process_running);
                if (getPCBpriority(current_process_running) < 3) {
                    lowerPCBpriority(current_process_running);
                }
                enqueueBasedOnPriority(current_process_running, systemQueue, highQueue, mediumQueue, lowQueue);
            }
            PCB *mediumProcess = dequeue(mediumQueue);
            if (getPCBstate(mediumProcess) == READY) {
                // Process is ready to run
                startProcess(mediumProcess);
            }
            else {
                // Process has been previously suspended and must be restarted
                restartProcess(mediumProcess);
            }
            current_process_running = mediumProcess;
            system_process_running = 0;
        }
        else if (sizeQUEUE(lowQueue) > 0) {
            // low priority process waiting to be run
            if (current_process_running) {
                suspendProcess(current_process_running);
                if (getPCBpriority(current_process_running) < 3) {
                    lowerPCBpriority(current_process_running);
                }
                enqueueBasedOnPriority(current_process_running, systemQueue, highQueue, mediumQueue, lowQueue);
            }
            PCB *lowProcess = dequeue(lowQueue);
            if (getPCBstate(lowProcess) == READY) {
                // Process is ready to run
                startProcess(lowProcess);
            }
            else {
                // Process has been previously suspended and must be restarted
                restartProcess(lowProcess);
            }
            current_process_running = lowProcess;
            system_process_running = 0;
        }
        if (current_process_running) {
            if (getPCBprocessorTime(current_process_running) > 0) {
                decrementPCBprocessorTime(current_process_running);
            }
            else {
                terminateProcess(current_process_running);
                freePCB(current_process_running);
                current_process_running = 0;
            }
        }
        current_time++; // Increment current time
        sleep(TIME_QUANTUM);
    }

    // Free memory and exit program
    freeSLL(dispatchList);
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
    SLL *dispatchList = newSLL(displayPCB, freePCB);
    int arrival, priority, processor;
    char trash; // for commas
    arrival = readInt(fp);
    while (!feof(fp)) {
        trash = readChar(fp);
        priority = readInt(fp);
        trash = readChar(fp);
        processor = readInt(fp);
        PCB *pcb = newPCB(arrival, priority, processor);
        insertSLL(dispatchList, sizeSLL(dispatchList), pcb);
        arrival = readInt(fp);
    }
    (void) trash; // trick compiler for unused variable warning
    fclose(fp);
    return dispatchList;
}

int processesRemain(SLL *dispatchList) {
    assert(dispatchList != 0);
    return sizeSLL(dispatchList);
}

void getArrivedProcesses(int current_time, SLL *dispatchList, QUEUE *sq, QUEUE *hq, QUEUE *mq, QUEUE *lq) {
    assert(current_time >= 0);
    assert(dispatchList != 0);
    assert(system != 0);
    assert(hq != 0);
    assert(mq != 0);
    assert(lq != 0);
    while (processesRemain(dispatchList) && getPCBarrivalTime(getSLL(dispatchList, 0)) <= current_time) {
        PCB *p = removeSLL(dispatchList, 0);
        setPCBstate(p, READY);
        enqueueBasedOnPriority(p, sq, hq, mq, lq);
    }
}

void enqueueBasedOnPriority(PCB *p, QUEUE *sq, QUEUE *hq, QUEUE *mq, QUEUE *lq) {
        switch (getPCBpriority(p)) {
            case SYSTEM_PRIORITY:
                enqueue(sq, p);
                break;
            case HIGH_PRIORITY:
                enqueue(hq, p);
                break;
            case MEDIUM_PRIORITY:
                enqueue(mq, p);
                break;
            case LOW_PRIORITY:
                enqueue(lq, p);
                break;
            default:
                enqueue(lq, p);
                break;
        }
}

void lowerPCBpriority(PCB *p) {
    assert(p != 0);
    setPCBpriority(p, getPCBpriority(p) + 1);
}

void startProcess(PCB *process) {
    pid_t pid = fork();
    if (pid == 0) {
        char *args[1];
        args[0] = NULL;
        execvp("./process", args);
    }
    else {
        setPCBpid(process, pid);
    }
    setPCBstate(process, RUNNING);
}

void terminateProcess(PCB *process) {
    kill(getPCBpid(process), SIGINT);
    waitpid(getPCBpid(process), NULL, WUNTRACED);
}

void suspendProcess(PCB *process) {
    kill(getPCBpid(process), SIGTSTP);
    waitpid(getPCBpid(process), NULL, WUNTRACED);
    if (getPCBpriority(process) < 3) {
        setPCBpriority(process, getPCBpriority(process) + 1);
    }
    setPCBstate(process, WAITING);
}

void restartProcess(PCB *process) {
    kill(getPCBpid(process), SIGCONT);
    setPCBstate(process, RUNNING);
}
