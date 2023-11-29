CC = gcc
CFLAGS = -O3 -Wall -Werror

.PHONY: all clean

all: main main_fixedpoint

main: main.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c $<

main_fixedpoint: main_fixedpoint.o
	$(CC) $(CFLAGS) -o $@ $^

main_fixedpoint.o: main_fixedpoint.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f main main.o main_fixedpoint main_fixedpoint.o