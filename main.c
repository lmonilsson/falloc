#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

void print_usage(const char *prefix) {
	fprintf(stderr, "Usage: %s SIZE FILE\n", prefix);
}

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

	if (errno) {
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

int do_allocate(int fd, long long size)
{
	if (ftruncate(fd, size) < 0) {
		perror("ftruncate");
		return 1;
	}

	return 0;
}

int allocate(const char *filename, long long size) {
	int fd = open(filename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		perror(filename);
		return 1;
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

	return allocate(argv[2], size);
}
