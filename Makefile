# Copyright (c) 2020 Davi Poyastro
# Licensed under the MIT License.

CFLAGS  := -g
LDFLAGS := -lm

.PHONY: all clean
all: main.elf test.elf

main.elf: hsnr.o main.o
	gcc $^ $(LDFLAGS) -o $@

test.elf: hsnr.o test.o
	gcc $^ $(LDFLAGS) -o $@
    
%.o: %.c
	gcc -c $< $(CFLAGS) -o $@

clean: 
	rm -f *.o *.elf