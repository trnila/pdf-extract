BINARY=extract

CC=gcc
CFLAGS=-g -Wall -Wextra

all: $(BINARY)

$(BINARY): main.o patterns.o split.o pdf2text.o
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(BINARY) *.o