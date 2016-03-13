#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int pdf2text(const char* input) {
	int pipes[2];
	pipe(pipes);
	if(fork() == 0) {
		close(pipes[0]);

		int another[2];
		pipe(another);
		if(fork() == 0) {
			close(pipes[1]);
			dup2(another[1], 1);

			execlp("pdftotext", "pdftotext", input, "-", NULL);
			perror("execl: ");
			exit(1);
		}

		close(another[1]);
		dup2(pipes[1], 1);
		dup2(another[0], 0);

		execlp("iconv", "iconv", "-t", "ASCII//TRANSLIT", NULL);
		perror("execl: ");
		exit(1);
	}
	close(pipes[1]);

	return pipes[0];
}