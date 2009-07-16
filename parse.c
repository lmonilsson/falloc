#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

long long parse_size(const char *str) {
	char *ptr;
	long long size;

	errno = 0;
	size = strtoll(str, &ptr, 0);

	if (errno == ERANGE || size < 0) {
		fprintf(stderr,
			"Size out of range; it must be within 0 and %d bytes.\n",
			LLONG_MAX);
		return -1;
	}

	if (errno || ptr == str) {
		fprintf(stderr, "Invalid size specified.\n");
		return -1;
	}

	if (*ptr != '\0') {
		char c = tolower(*ptr);

		if (c == 'k')
			size *= 1024;
		else if (c == 'm')
			size *= 1024 * 1024;
		else if (c == 'g')
			size *= 1024 * 1024 * 1024;
		else {
			fprintf(stderr, "Invalid suffix specified.\n");
			return -1;
		}
	}

	return size;
}

