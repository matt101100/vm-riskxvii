TARGET = vm_riskxvii

CC = gcc

CFLAGS     = -c -Wall -Wvla -Werror -O0 -g -std=c11
ASAN_FLAGS = -fsanitize=address
SRC        = vm_riskxvii.c
OBJ        = $(SRC:.c=.o)

all:$(TARGET)

$(TARGET):$(OBJ)
	$(CC) -s -Os -o $@ $(OBJ)

.SUFFIXES: .c .o

.c.o:
	 $(CC) -c -s -Os $<

run:
	./$(TARGET) $(ARGS)

test:
	echo what are we testing?!

clean:
	rm -f *.o *.obj $(TARGET)
	rm debug

debug:$(TARGET)
	$(CC) $(ASAN_FLAGS) -o $@ $(OBJ)

