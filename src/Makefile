CC=gcc
CFLAGS=-Wall

all: clean connect parser
	mkfifo net/in net/out

connect: connect.c
	$(CC) $(CFLAGS) $? -o net/$@

parser: parser.c
	$(CC) $(CFLAGS) $? -o message/$@

clean:
	rm -f net/in net/out message/parser net/connect
