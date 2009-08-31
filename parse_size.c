#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <limits.h>

#include "options.h"

static long long checked_multiply(long long start, int times)
{
	int multiplier = fallocate_options.byte_multiplier;

	while (times--) {
		if (start > LLONG_MAX / multiplier)
			return -1;
		else
			start *= multiplier;
	}

	return start;
}

long long parse_size(const char *str)
{
	char *ptr;
	long long size;

	errno = 0;
	size = strtoll(str, &ptr, 0);

	if (errno == ERANGE || size < 0) {
		fprintf(stderr,
			"Size out of range; it must be within 0 and %lld bytes.\n",
			LLONG_MAX);
		return -1;
	}

	if (errno || ptr == str) {
		fprintf(stderr, "Invalid size specified.\n");
		return -1;
	}

	if (*ptr != '\0') {
		if (!strcasecmp(ptr, "k"))
			size = checked_multiply(size, 1);
		else if (!strcasecmp(ptr, "m"))
			size = checked_multiply(size, 2);
		else if (!strcasecmp(ptr, "g"))
			size = checked_multiply(size, 3);
		else {
			fprintf(stderr, "Invalid suffix \"%s\" specified.\n", ptr);
			return -1;
		}

		if (size == -1) {
			fprintf(stderr,
				"Size out of range; it must be within 0 and %lld bytes.\n",
				LLONG_MAX);
			return -1;
		}
	}

	return size;
}

