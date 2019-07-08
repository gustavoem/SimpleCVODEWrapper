CFLAGS = -g -Wall -O2 -ansi -pedantic -Wno-unused-result
CC = gcc
LDFLAGS = -I/usr/local/include -L/usr/local/lib -lsundials_cvode -lsundials_nvecserial -lm /usr/lib/x86_64-linux-gnu/librt.so -Wl,-rpath,/usr/local/lib

test: test.o scvodew.o
	$(CC) $(CFLAGS) test.o  scvodew.o -o test $(LDFLAGS)

test.o: test.c
	$(CC) $(CFLAGS) -c test.c

scvodew.o: scvodew.h scvodew.c
	$(CC) $(CFLAGS) -c scvodew.c

clean:
	rm *.o
