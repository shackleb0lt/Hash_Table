CXX     = g++

CXXFLAGS = -O1 -std=c++14 #-Wall -g3

all: file.o hash.o run

run: mytest.cpp file.o hash.o
	$(CXX) $(CXXFLAGS) mytest.cpp file.o hash.o -o run

file.o: file.cpp file.h
	$(CXX) $(CXXFLAGS) -c file.cpp -o file.o

hash.o: hash.cpp hash.h
	$(CXX) $(CXXFLAGS) -c hash.cpp -o hash.o

memtest:
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ./run
	
clean:
	rm -f file.o hash.o run