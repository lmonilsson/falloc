#include <stdio.h>
#include <limits.h>

#include "parse.h"

int test_parse_size(const char *str, long long expected)
{
	long long size = parse_size(str);

	if (size == expected) {
		printf("* '%s' gave the expected result, '%lld'\n", str, size);
		return 0;
	} else {
		printf("* '%s' gave the incorrect result, '%lld'\n", str, size);
		return 1;
	}
}

int main(int argc, char *argv[])
{
	int errors = 0;

	errors += test_parse_size("1", 1);
	errors += test_parse_size("10", 10);
	errors += test_parse_size("1024", 1024);

	errors += test_parse_size("1k", 1024);
	errors += test_parse_size("1K", 1024);
	errors += test_parse_size("10k", 1024 * 10);
	errors += test_parse_size("1024k", 1024 * 1024);

	errors += test_parse_size("1m", 1024 * 1024);
	errors += test_parse_size("1M", 1024 * 1024);
	errors += test_parse_size("10m", 1024 * 1024 * 10);
	errors += test_parse_size("1024m", 1024 * 1024 * 1024);

	errors += test_parse_size("1g", 1024 * 1024 * 1024);
	errors += test_parse_size("1G", 1024 * 1024 * 1024);
	errors += test_parse_size("10g", 1024ll * 1024 * 1024 * 10);
	errors += test_parse_size("1024g", 1024ll * 1024 * 1024 * 1024);

	errors += test_parse_size("a", -1);
	errors += test_parse_size("1a", -1);
	errors += test_parse_size("a1", -1);
	errors += test_parse_size("", -1);
	errors += test_parse_size("1.5", -1);
	errors += test_parse_size("1,5", -1);
	errors += test_parse_size("-1", -1);
	errors += test_parse_size("9223372036854775808", -1); // Oversized

	printf("\n");
	if (errors)
		printf("%d errors found.\n", errors);
	else
		printf("No errors found,\n");

	return 0;
}
