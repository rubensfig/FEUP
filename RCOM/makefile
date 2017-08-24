CC = gcc
DEPS = download.h
#CFLAGS = -Wall

default: download

download: download.o
	$(CC) -o download download.o $(CFLAGS)
	
download.o: download.c download.h
	$(CC) -c download.c $(CFLAGS)

clean:
	rm -f *.o download readme.txt