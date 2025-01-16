cc=gcc
target = main

objects = main.o print.o save.o

$(target) : $(objects)
	$(cc) -o $(target) $(objects)

$.o : $.c
	$(cc) -c -o $@ $<

main.o print.o save.o : header.h

.PHONY : clean
clean :
	rm $(target) $(objects)
