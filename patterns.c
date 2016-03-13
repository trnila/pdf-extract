#include <stddef.h>
#include <malloc.h>
#include <memory.h>
#include "patterns.h"

typedef struct Pattern {
	char* pattern;
	size_t pos;
	size_t length;
	struct Pattern *next;
} Pattern;

Pattern *root = NULL;

void pattern_add(char *pattern) {
	Pattern **pat = &root;
	while(*pat != NULL) {
		pat = &(*pat)->next;
	}

	*pat = (Pattern*) malloc(sizeof(Pattern));

	(*pat)->pos = 0;
	(*pat)->length = strlen(pattern);

	(*pat)->pattern = (char*) malloc(((*pat)->length + 1) * sizeof(char));
	memcpy((*pat)->pattern, pattern, (*pat)->length + 1);
}

const char* pattern_any_match(char letter) {
	Pattern *pattern = root;
	while(pattern != NULL) {
		size_t thisPos = pattern->pos + 1;

		if(thisPos < pattern->length && pattern->pattern[thisPos] == letter) {
			if(thisPos + 1 == pattern->length) {
				pattern->pos = 0;
				return pattern->pattern;
			}
			pattern->pos++;
		} else {
			pattern->pos = 0;
		}

		pattern = pattern->next;
	}

	return NULL;
}

void pattern_reset() {
	Pattern *pattern = root;
	while(pattern != NULL) {
		pattern->pos = 0;
		pattern = pattern->next;
	}
}