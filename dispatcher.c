#include "pcb.h"
#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <filepath>\n", argv[0]);
        exit(1);
    }

    FILE *fp = fopen(argv[1], "r");
    char trash;
    int arrival, priority, processor;
    arrival = readInt(fp);
    while (!feof(fp)) {
        trash = readChar(fp); // get comma
        priority = readInt(fp);
        trash = readChar(fp); // get comma
        processor = readInt(fp);
        PCB *pcb = newPCB(arrival, priority, processor);
        displayPCB(pcb, stdout);
        arrival = readInt(fp);
    }
    (void) trash; // trick compiler

    return 0;
}
