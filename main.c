#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "parse.h"

void print_usage(const char *prefix) {
	fprintf(stderr, "Usage: %s SIZE FILE\n", prefix);
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
	int fd = open(filename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		perror(filename);
		return -1;
	}

	int ret = do_allocate(fd, size);

	close(fd);
	return ret;
}

int main(int argc, char *argv[])
{
	if (argc != 3) {
		print_usage(argv[0]);
		return 1;
	}

	long long size = parse_size(argv[1]);
	if (size < 0)
		return 1;

	return (allocate(argv[2], size) == 0) ? 0 : 1;
}
