CFLAGS = -g -Wall -O2 -ansi -pedantic -Wno-unused-result
CC = gcc
LDFLAGS = -I/usr/local/include -L/usr/local/lib -lsundials_cvode -lsundials_nvecserial -lm /usr/lib/x86_64-linux-gnu/librt.so -Wl,-rpath,/usr/local/lib

main: main.o scvodew.o
	$(CC) main.o  scvodew.o -o main $(LDFLAGS)

main.o: main.c
	$(CC) -c main.c

scvodew.o: scvodew.h scvodew.c
	$(CC) -c scvodew.c

clean:
	rm *.o
