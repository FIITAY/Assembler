CFLAGS=-Wall -pedantic -ansi

OBJS = assembler.o base64.o error.o firstLoop.o lineParse.o secondLoop.o

#compile command for mycomp
assembler: $(OBJS)
	gcc $(CFLAGS) -g $^ -o $@

%.o: %.c
	gcc $(CFLAGS) -g -c $^ 

# cleanup clean:
clean:
	rm -f *.o *.d *.gch