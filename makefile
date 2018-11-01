OOPTS = -Wall -Wextra -std=c99 -g -c
LOPTS = -Wall -Wextra -std=c99 -g

all:	process dispatcher

process: 	sigtrap.c
	gcc -g sigtrap.c -o process -Wall

dispatcher:	dispatcher.o scanner.o sll.o queue.o
	gcc $(LOPTS) dispatcher.o scanner.o sll.o queue.o -o dispatcher

dispatcher.o: 	dispatcher.c queue.h sll.h
	gcc $(OOPTS) dispatcher.c

scanner.o: 	scanner.c scanner.h
	gcc $(OOPTS) scanner.c

sll.o:	sll.c sll.h
	gcc $(OOPTS) sll.c

queue.o:	queue.c queue.h sll.h
	gcc $(OOPTS) queue.c

test:	all
	./dispatcher

valgrind:	all

clean:
	rm -f *.o vgcore.* ./dispatcher ./process
