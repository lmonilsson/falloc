#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <strings.h>

#include "options.h"

struct fallocate_options fallocate_options = {
	.byte_multiplier = 1024,
	.force = 0,
	.help = 0
};

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

static long long parse_size(const char *str)
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

	if (!strcasecmp(ptr, "k"))
		size = checked_multiply(size, 1);
	else if (!strcasecmp(ptr, "m"))
		size = checked_multiply(size, 2);
	else if (!strcasecmp(ptr, "g"))
		size = checked_multiply(size, 3);
	else if (*ptr) {
		fprintf(stderr, "Invalid suffix \"%s\" specified.\n", ptr);
		return -1;
	}

	if (size == -1) {
		fprintf(stderr,
			"Size out of range; it must be within 0 and %lld bytes.\n",
			LLONG_MAX);
		return -1;
	}

	return size;
}

int parse_options(int argc, char *argv[])
{
	int c;
	while ((c = getopt(argc, argv, "hbsfn")) != -1) {
		switch (c) {
		case 'h':
			fallocate_options.help = 1;
			return 0;
		case 'b':
			fallocate_options.byte_multiplier = 1024;
			break;
		case 's':
			fallocate_options.byte_multiplier = 1000;
			break;
		case 'f':
			fallocate_options.force = 1;
			break;
		case 'n':
			fallocate_options.force = 0;
			break;
		default:
			return -1;
		}
	}

	if (optind > argc - 2) {
		fprintf(stderr, "SIZE or FILE missing.\n");
		return -1;
	}

	long long size = parse_size(argv[optind]);
	if (size < 0)
		return -1;

	fallocate_options.size = size;
	fallocate_options.filename = argv[optind + 1];

	return 0;
}
