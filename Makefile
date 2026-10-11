CC     = gcc
CFLAGS = -Wall -Wextra

voidfetch: main.c
	$(CC) $(CFLAGS) main.c -o voidfetch

run: voidfetch
	./voidfetch

clean:
	rm -f voidfetch a.out

.PHONY: run clean
