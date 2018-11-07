main.o: functions.h
	gcc -c -Wall main.c

build: functions.h
	gcc -o assistent main.c -lpthread -Wall

run:
	./assistent

clean:
	rm sudoku main.o
