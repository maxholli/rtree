CC=g++
WARN=-Wall
TYP=c++17

btreemake: main.o btree.o llist.o
	$(CC) $(WARN) -std=$(TYP) -o exe main.o btree.o llist.o

.PHONY: clean

clean:
	rm *.o
	rm exe