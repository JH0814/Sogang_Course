cc=gcc
target = animal_exe
objects = main.o dog.o blackcow.o turtle.o
$(target) : $(objects)
	$(cc) -o $(target) $(objects)

$.o : %.c
	$(cc) -c -o $@ $<

main.o dog.o blackcow.o turtle.o : dog.h blackcow.h turtle.h

.PHONY : clean
clean :
	rm $(target) $(objects)
