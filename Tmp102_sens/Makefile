CC ?= $(CROSS_COMPILE)gcc
CFLAGS ?= -g -Wall -Werror
SRC = temperature.c
OUTPUT = temperature

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT)
clean:
	rm -rf *o $(OUTPUT) *.elf *.map
