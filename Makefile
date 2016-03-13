BINARY=extract

all: $(BINARY)

$(BINARY): main.o patterns.o split.o pdf2text.o
	gcc $^ -o $@ -g

.PHONY: clean
clean:
	rm -f $(BINARY) *.o