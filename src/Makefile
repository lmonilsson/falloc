CC = gcc
CFLAGS = -g -std=gnu99

SRCS = main.c parse_size.c options.c
OBJS = $(SRCS:.c=.o)
EXEC = ../falloc

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

main.o: options.h
parse_size.o: parse_size.h options.h
options.o: parse_size.h options.h

clean:
	rm -f $(EXEC)
	rm -f $(OBJS)
