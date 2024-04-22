CC= g++
CFLAGS= -std=gnu++20 -Wall -Wextra
OBJ= main.o datagram.o oknoPrzesuwne.o
DEPS= datagram.hpp oknoPrzesuwne.hpp

TARGET=transport

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ)

distclean:
	rm -f $(OBJ) $(TARGET)