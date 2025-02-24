CC=g++
FLAGS=-std=c++11 -Wall 

all: main

main: main.o 
	$(CC) $(FLAGS) main.o -o main

main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp

clean:
	rm -rf *.o main


