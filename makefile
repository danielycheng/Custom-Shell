#Daniel Cheng - dyc8av
#February 5, 2015
#makefile for HW1 in order to make grading easier

all: main.cpp parse.cpp 
	g++ main.cpp parse.cpp -o msh

clean:
	-rm -f *.o *~
