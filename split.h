#pragma once

typedef struct {
	const char* file;
	int *pages;
	size_t last;
	size_t pagesCount;
} Split;

void split_init(Split *split, const char *file);
void split_addpage(Split *split, int page);
void split_split(Split *split, const char *out);
void split_free(Split *split);
