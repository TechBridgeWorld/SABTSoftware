CC = gcc
CFLAGS = -g -DDEBUGMODE
OBJECTS = debug.o io.o mode_6.o audio.o script_common.o letter_globals.o common.o \
datastructures.o script_english.o script_hindi.o script_kannada.o script_digits.o

all : debug

debug : $(OBJECTS)
	$(CC) -g $(OBJECTS) -o debug

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *~ *.o debug