cc = gcc
cflags = -W -g
target = testlib

objects = main.o list.o hash.o bitmap.o hex_dump.o debug.o

$(target) : $(objects)
	$(cc) $(cflags) -o $(target) $(objects)

%.o : %.c
	$(cc) $(cflags) -c -o $@ $<

main.o : main.c list.h hash.h bitmap.h
list.o : list.c list.h
hash.o : hash.c hash.h
bitmap.o : bitmap.c bitmap.h
hex_dump.o : hex_dump.c hex_dump.h
debug.o : debug.c debug.h


.PHONY : clean
clean :
	rm -f $(target) $(objects)