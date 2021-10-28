CC=g++
WARN=-Wall
TYP=c++17

btreemake: main.o btree.o
	$(CC) $(WARN) -std=$(TYP) -o btree_test.exe main.o btree.o

.PHONY: clean

clean:
	rm *.o
	rm *.exe