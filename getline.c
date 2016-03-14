#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
	char buffer[100];
	int pos;
	int len;
} Buffer;

void p(char*msg, char*x, int s, int p) {
	printf("%s: '", msg);
	for(int i = s; i < p; i++) {
		printf("%c", x[i]);
	}
	printf("'\n");
}

Buffer *buffers = NULL;
int readline(int fd, char *buf, size_t len) {
	if(buffers == NULL) {
		buffers = (Buffer*) malloc(sizeof(Buffer));
		buffers->pos = buffers->len = 0;
	}
	Buffer *buffer = buffers;

	do {
		for(; buffer->pos < buffer->len; buffer->pos++) {
			if(buffer->buffer[buffer->pos] == '\n') {
				memcpy(buf, buffer->buffer, buffer->pos);
				buf[buffer->pos] = 0;

				size_t remaining = buffer->len - buffer->pos - 1;
				memcpy(buffer->buffer, buffer->buffer + buffer->pos + 1, remaining);

				int pos = buffer->pos;
				buffer->pos = 0;
				buffer->len -= pos;
				return pos;
			}
		}

	//	printf(">>%s<<\n", buffer->buffer);

		ssize_t r = read(fd, buffer->buffer + buffer->pos, sizeof(buffer->buffer) - buffer->pos);
		if(r == 0) {
			return 0;
		} else {
			buffer->len += r;
		}
	} while(1);
}
