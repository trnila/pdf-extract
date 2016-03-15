#include <sys/types.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

typedef struct Buffer {
	int fd;
	char buffer[2048];
	size_t pos;
	size_t end;
	struct Buffer *next;
} Buffer;

Buffer *buffers = NULL;

Buffer* getBuffer(int fd) {
	Buffer **buffer = &buffers;
	while(*buffer != NULL) {
		if((*buffer)->fd == fd) {
			return *buffer;
		}
	}

	// create a new buffer
	Buffer *newBuffer = (Buffer*) malloc(sizeof(Buffer));
	newBuffer->pos = newBuffer->end = 0;
	newBuffer->fd = fd;
	newBuffer->next = buffers;
	buffers = newBuffer;

	return newBuffer;
}

int readline(int fd, char *buf, size_t len) {
	if(buf == NULL || len <= 0) {
		errno = EINVAL;
		return -1;
	}

	Buffer *buffer = getBuffer(fd);

	ssize_t r;
	do {
		size_t start = buffer->pos;
		for(; buffer->pos < buffer->end; buffer->pos++) {
			if(buffer->buffer[buffer->pos] == '\n') {
				size_t lineLength = buffer->pos - start;

				// line is greater than provided buffer
				if(lineLength >= len) {
					errno = ENOBUFS;
					return -1;
				}

				// copy line to provided buffer
				memcpy(buf, buffer->buffer + start, lineLength);
				buf[lineLength] = 0;
				buffer->pos++;

				return (int) lineLength;
			}
		}
		

		size_t remaining = buffer->end - start;
		size_t bufferRemaining = sizeof(buffer->buffer) - remaining - 1;
		if(bufferRemaining <= 0) {
			errno = ENOBUFS;
			return -1;
		}

		// move part of line to the beginning of the buffer
		memmove(buffer->buffer, buffer->buffer + start, remaining);

		// read-append another chunk to the buffer
		r = read(fd, buffer->buffer + remaining, bufferRemaining);
		buffer->end = remaining + r;
		buffer->pos = 0;
	} while(r > 0);

	return 0;
}
