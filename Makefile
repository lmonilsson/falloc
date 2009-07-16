CC = gcc
CFLAGS = -g -std=gnu99

EXEC = fallocate
SRCS = main.c
OBJS = $(SRCS:.c=.o)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(EXEC) $(OBJS)
