CFLAGS = -Wall
CC = gcc

main: sender.o reciver.o

clean:
	rm -f sender.o reciver.o sender reciver

test:  main
	@echo -e "\n\nTESTING\n\n"
	./main 22
	./main -9
	./main 0
	./main cos

all: main test
