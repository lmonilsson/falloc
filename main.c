#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "options.h"

void print_usage(const char *program) {
	fprintf(stderr, "Usage: %s [OPTIONS] SIZE FILE\n", program);
}

int do_allocate(int fd, long long size)
{
	if (ftruncate(fd, size) < 0) {
		perror("ftruncate");
		return -1;
	}

	return 0;
}

int allocate(const char *filename, long long size) {
	int flags = O_WRONLY | O_CREAT;

	if (!fallocate_options.force)
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

	ret = allocate(fallocate_options.filename, fallocate_options.size);

	return (ret == 0) ? 0 : 1;
}
