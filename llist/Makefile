CC=g++
WARN=-Wall
TYP=c++17

llistmake: main.o
	$(CC) $(WARN) -std=$(TYP) -o llist_test.exe main.o 

.PHONY: clean

clean:
	rm *.o
	rm *.exe