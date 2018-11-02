OOPTS = -Wall -Wextra -std=c99 -g -c
LOPTS = -Wall -Wextra -std=c99 -g

all:	process dispatcher

process: 	sigtrap.c
	gcc -g sigtrap.c -o process -Wall

dispatcher:	dispatcher.o pcb.o scanner.o sll.o queue.o
	gcc $(LOPTS) dispatcher.o pcb.o scanner.o sll.o queue.o -o dispatcher

dispatcher.o: 	dispatcher.c pcb.h queue.h sll.h
	gcc $(OOPTS) dispatcher.c

scanner.o: 	scanner.c scanner.h
	gcc $(OOPTS) scanner.c

sll.o:	sll.c sll.h
	gcc $(OOPTS) sll.c

queue.o:	queue.c queue.h sll.h
	gcc $(OOPTS) queue.c

pcb.o: 	pcb.c pcb.h
	gcc $(OOPTS) pcb.c

test:	all
	./dispatcher ./sampleinput.txt

valgrind:	all
	clear
	valgrind ./dispatcher ./sampleinput.txt

clean:
	rm -f *.o vgcore.* ./dispatcher ./process
