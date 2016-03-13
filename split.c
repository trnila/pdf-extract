#include <stddef.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include "split.h"

void split_init(Split *split, const char *file) {
	split->file = file;
	split->pages = NULL;
	split->last = 0;
	split->pagesCount = 0;
}

void split_addpage(Split *split, int page) {
	if(split->last >= split->pagesCount) {
		split->pagesCount += 1;
		split->pages = realloc(split->pages, sizeof(split->pages[0]) * split->pagesCount);
	}

	split->pages[split->last++] = page;
}

void split_split(Split *split, const char *out) {
	//TODO: buffer should have dynamic size!
	char buffer[1024];
	int pos = 0;
	for(int i = 0; i < split->last; i++) {
		pos += sprintf(buffer + pos, "%d,", split->pages[i]);
	}
	buffer[pos-1] = '\0';

	if(fork() == 0) {
		execlp("pdfjam", "pdfjam", "--fitpaper", "true", split->file, buffer, "-o", out, NULL);
		perror("execl: ");
		exit(1);
	}

	while(wait(NULL) > 0);
}

void split_free(Split *split) {
	if(split->pages) {
		free(split->pages);
	}

	split->file = NULL;
	split->pages = NULL;
	split->last = 0;
	split->pagesCount = 0;
}