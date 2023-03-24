TARGET = vm_riskxvii

CC = gcc

CFLAGS     = -c -Wall -Wvla -Werror -O0 -g
ASAN_FLAGS = -fsanitize=address
SRC        = vm_riskxvii.c
OBJ        = $(SRC:.c=.o)

all:$(TARGET)

debug:$(TARGET)
	$(CC) $(ASAN_FLAGS) -o $@ $(OBJ)

$(TARGET):$(OBJ)
	$(CC) -Os -s -o $@ $(OBJ)

.SUFFIXES: .c .o

.c.o:
	 $(CC) -Os -s $(CFLAGS) $<

run:
	./$(TARGET) $(ARGS)

test:
	echo what are we testing?!

clean:
	rm -f *.o *.obj $(TARGET)
	rm -f debug
