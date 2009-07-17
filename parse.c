#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

static long long multiply_by_1024(long long start, int times)
{
	while (times--) {
		if (start > LLONG_MAX / 1024)
			return -1;
		else
			start *= 1024;
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
		char suffix = tolower(*ptr);

		if (suffix == 'k')
			size = multiply_by_1024(size, 1);
		else if (suffix == 'm')
			size = multiply_by_1024(size, 2);
		else if (suffix == 'g')
			size = multiply_by_1024(size, 3);
		else {
			fprintf(stderr, "Invalid suffix '%c' specified.\n", suffix);
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

