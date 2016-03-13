#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wait.h>
#include "patterns.h"
#include "split.h"
#include "pdf2text.h"

int main(int argc, char **argv) {
	if(argc != 3) {
		printf("Usage: %s input.pdf output.pdf\n", argv[0]);
		return 1;
	}
	
	const char* input = argv[1];
	const char* output = argv[2];

	int fd = pdf2text(input);

	pattern_add("Definice");
	Split split;
	split_init(&split, input);

	char buffer[30];
	int page = 1;
	ssize_t r;
	const char *match;
	while((r = read(fd, buffer, sizeof(buffer))) > 0) {
		for(int i = 0; i < r; i++) {
			if(buffer[i] == '\f') {
				page++;
				printf("Page %d\n", page);
				pattern_reset();
			} else if((match = pattern_any_match(buffer[i])) != NULL) {
				printf("Any match! %s\n", match);
				split_addpage(&split, page);
			}
		}
	}

	split_split(&split, output);

	while(wait(NULL) > 0);
	return 0;
}