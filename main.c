#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wait.h>
#include <getopt.h>
#include <ctype.h>
#include "patterns.h"
#include "split.h"
#include "pdf2text.h"
#include "log.h"

void printHelp(const char* programName) {
	printf("Usage: %s input.pdf output.pdf\n", programName);
	printf("  -h       show this help\n");
	printf("  -v       raise verbosity level, -v -v -v is max debug verbosity\n");
	printf("  -w word  add word to the list of searching patterns\n");
}

int main(int argc, char **argv) {
	int c;
	while ((c = getopt (argc, argv, "w:vh")) != -1) {
		switch (c) {
			case 'v':
				verbosity++;
				break;
			case 'w':
				pattern_add(optarg);
				log_debug("Adding word %s\n", optarg);
				break;
			case 'h':
				printHelp(argv[0]);
				return EXIT_SUCCESS;
			case '?':
				log_debug("Unknown option `-%c'.\n", optopt);
				return EXIT_FAILURE;
			default:
				abort();
		}
	}

	if(argc - optind != 2) {
		printHelp(argv[0]);
		return EXIT_FAILURE;
	}

	const char* input = argv[optind];
	const char* output = argv[optind + 1];

	int fd = pdf2text(input);
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
				log_debug("Page %d\n", page);
				pattern_reset();
			} else if(isalpha(buffer[i]) && (match = pattern_any_match((char) tolower(buffer[i]))) != NULL) {
				log_debug("Match of %s found\n", match);
				split_addpage(&split, page);
			}
		}
	}

	split_split(&split, output);

	while(wait(NULL) > 0);

	pattern_free();
	split_free(&split);
	return 0;
}