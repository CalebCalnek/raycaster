CC = gcc
CFLAGS = -Wall -g -I.
LDFLAGS = -lm `sdl2-config --cflags --libs`
SRC = $(wildcard src/*.c)

.PHONY: all clean

all: raycaster

clean:
	rm -f raycaster

raycaster: $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
