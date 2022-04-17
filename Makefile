OPT     = -O3

CC        = gcc
CCFLAGS   = -Wall $(OPT) -std=gnu99
LDFLAGS   = 

TARGET    = stm32-crc32

all: stm32-crc32


stm32-crc32: src/stm32-crc32.o
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) -c $< $(CCFLAGS) -o $(*).o

clean:
	rm -f src/*.o $(TARGET)*
