CC ?= $(CROSS_COMPILE)gcc
CFLAGS ?= -g -Wall -Werror
SRC = humidity.c wiringPi.c
OUTPUT = humidity

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT)
clean:	
	rm -rf *o $(OUTPUT) *.elf *.map
