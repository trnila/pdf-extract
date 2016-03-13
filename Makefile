BINARY=extract

all: $(BINARY)

$(BINARY): main.c patterns.c split.c
	gcc $^ -o $@ -g

.PHONY: clean
clean:
	rm -f $(BINARY) *.o