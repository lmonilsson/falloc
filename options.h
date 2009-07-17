#ifndef FALLOCATE_OPTIONS_H
#define FALLOCATE_OPTIONS_H

struct fallocate_options
{
	int byte_multiplier; // Size of kilo, mega, etc (1000 or 1024).
	int force; // Whether to force allocation on already existing files.
	int help; // Whether to show the help text.

	long long size;
	const char *filename;
};

extern struct fallocate_options fallocate_options;

int parse_options(int argc, char *argv[]);

#endif
