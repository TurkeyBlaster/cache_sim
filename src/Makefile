CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -fsanitize=undefined,address -std=gnu17

OBJS = cache.o handle_args.o hashmap.o linkedlist.o

all : sim
	./sim

sim : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) sim.c -o sim
	rm -f *.o

$(OBJS) : %.o : %.h
	$(CC) $(CFLAGS) -c $*.c

.PHONY : clean

clean :
	rm -f *.o
	rm -f sim
	rm -f `find . -maxdepth 1 -type f -executable`