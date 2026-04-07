CC = gcc
CFLAGS = -Wall -Wextra -pthread 
TARGET = task3
SRCS = main.c
OBJS = $(SRCS:.c=.o)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET) *.txt
