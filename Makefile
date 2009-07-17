CC = gcc
CFLAGS = -g -std=gnu99

SRCS = main.c parse.c options.c
OBJS = $(SRCS:.c=.o)
EXEC = fallocate

TEST_SRCS = testing.c parse.c options.c
TEST_OBJS = $(TEST_SRCS:.c=.o)
TEST_EXEC = fallocate_test

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(TEST_EXEC): $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJS)

test: $(TEST_EXEC)
	./$(TEST_EXEC)

main.o: options.h
parse.o: parse.h options.h
options.o: parse.h options.h
testing.o: parse.h options.h

clean:
	rm -f $(EXEC)
	rm -f $(TEST_EXEC)
	rm -f $(OBJS)
