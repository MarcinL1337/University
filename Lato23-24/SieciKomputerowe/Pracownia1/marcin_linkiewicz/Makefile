CC = gcc
CFLAGS = -Wall -Wextra -std=gnu17
LDFLAGS =
SOURCES = icmp_receive.c icmp_send.c main.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = traceroute

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)

distclean: clean
	rm -f $(EXECUTABLE)

.PHONY: all clean distclean