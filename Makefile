# Compiler flags
CC = gcc
CFLAGS = -O3 -Wall -ansi

# Sources
SOURCES = main.c garage.c restriction.c graph.c p_queue.c h_table.c queue.c LinkedList.c defs.c

# Objects
OBJECTS = main.o garage.o restriction.o graph.o p_queue.o h_table.o queue.o LinkedList.o defs.o

autopark: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

main.o: main.c

garage.o: garage.c

restriction.o: restriction.c

graph.o: graph.c

p_queue.o: p_queue.c

h_table.o: h_table.c

queue.o: queue.c

LinkedList.o: LinkedList.c

defs.o: defs.c

clean:
	rm -f *.o *.~ autopark *.gch

depend::
	makedepend $(SOURCES)
