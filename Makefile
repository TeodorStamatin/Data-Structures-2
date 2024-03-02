CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -g -lm
LOAD=load_balancer
SERVER=server
UTILS=utils

.PHONY: build clean run

build: tema2

tema2: main.o $(LOAD).o $(SERVER).o $(UTILS).o
	$(CC) $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) $^ -c

$(SERVER).o: $(SERVER).c $(SERVER).h
	$(CC) $(CFLAGS) $^ -c

$(LOAD).o: $(LOAD).c $(LOAD).h
	$(CC) $(CFLAGS) $^ -c

$(UTILS).o: $(UTILS).c $(UTILS).h
	$(CC) $(CFLAGS) $^ -c

run: tema2
	./tema2

clean:
	rm -f *.o tema2 *.h.gch

pack:
	zip -FSr 315CA_StamatinTeodor_Tema2SD.zip README.md Makefile *.c *.h