#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "options.h"

void print_usage(const char *program) {
	fprintf(stderr,
"Usage: %s [OPTIONS] SIZE FILE\n"
"Create empty FILE with size SIZE\n"
"SIZE is the number of bytes to allocate, which may have a size suffix (k, m, g)\n"
"\n"
"Options:\n"
"  -h  show this help message\n"
"  -b  use binary sizes (powers of 1024) for the suffixes     (default)\n"
"  -s  use SI sizes (powers of 1000) for the suffixes\n"
"  -n  do not perform the operation if FILE exists            (default)\n"
"  -f  force the operation even if FILE already exists, which\n"
"        may either extend or truncate the file\n"
			,program);
}

int do_allocate(int fd, long long size)
{
	int err;
	if ((err = posix_fallocate(fd, 0, size)) != 0) {
		fprintf(stderr, "%s\n", strerror(err));
		return -1;
	}

	return 0;
}

int allocate(const char *filename, long long size, int force) {
	int flags = O_WRONLY | O_CREAT;

	if (!force)
		flags |= O_EXCL;

	int fd = open(filename, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd < 0) {
		perror(0);
		return -1;
	}

	int ret = do_allocate(fd, size);

	close(fd);
	return ret;
}

int main(int argc, char *argv[])
{
	int ret = parse_options(argc, argv);

	if (fallocate_options.help || ret < 0) {
		print_usage(argv[0]);
		return (ret == 0) ? 0 : 1;
	}

	ret = allocate(fallocate_options.filename, fallocate_options.size,
	               fallocate_options.force);

	return (ret == 0) ? 0 : 1;
}
