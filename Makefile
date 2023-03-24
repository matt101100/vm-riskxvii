TARGET = vm_riskxvii

CC = gcc

CFLAGS     = -c
ASAN_FLAGS = -fsanitize=address
SRC        = vm_riskxvii.c
OBJ        = $(SRC:.c=.o)

all:$(TARGET)

$(TARGET):$(OBJ)
	$(CC) -o $@ $(OBJ)

.SUFFIXES: .c .o

.c.o:
	 $(CC) $(CFLAGS) $<

run:
	./$(TARGET) $(ARGS)

test:
	echo what are we testing?!

clean:
	rm -f *.o *.obj $(TARGET)