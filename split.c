#include <stddef.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <fcntl.h>
#include "split.h"
#include "log.h"

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

	pid_t pid = fork();
	if(pid <= -1) {
		perror("fork: ");
		exit(1);
	} else if(pid == 0) {
		int fd = open("/dev/null", O_WRONLY);
		dup2(fd, 1);
		dup2(fd, 2);

		execlp("pdfjam", "pdfjam", "--fitpaper", "true", split->file, buffer, "-o", out, NULL);
		perror("execl pdfjam");
		exit(1);
	}

	int status;
	waitpid(pid, &status, 0);
	if(status != 0) {
		log_error("pdfjam returned %d\n", status);
	} else {
		log_debug("pdfjam returned %d\n", status);
	}
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