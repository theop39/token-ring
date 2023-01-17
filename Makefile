CC=gcc
EXEC=petitChevaux
SRCS= $(wildcard *.c)
OBJS= $(SRCS:.c=.o)
CFLAGS=-I./include -Wall -Wextra -Werror
LDFLAGS=

all: $(EXEC)

petitChevaux: $(OBJS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< 

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)
