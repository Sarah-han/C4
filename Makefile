FLAGS = -Wall -Werror -g

all: frequency

frequency: main.o
	gcc $(FLAGS) main.o -o frequency
	
main.o: main.c
	gcc $(FLAGS) -c main.c

clean: 
	rm -r *.o frequency
