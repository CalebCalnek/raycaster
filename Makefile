CC = gcc
CFLAGS = -Wall -g -I.
LDFLAGS = `sdl2-config --cflags --libs`

.PHONY: all clean

all: myRaycaster

clean:
	rm -f myRaycaster

myRaycaster: main.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
