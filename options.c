#include <stdio.h>
#include <unistd.h>

#include "options.h"
#include "parse.h"

struct fallocate_options fallocate_options = {
	.byte_multiplier = 1024,
	.force = 0,
	.help = 0
};

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
