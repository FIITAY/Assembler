CFLAGS=-Wall -pedantic -ansi -MMD -MP

OBJS = assembler.o base64.o error.o firstLoop.o lineParse.o secondLoop.o
DEPS := $(OBJS:.o=.d)

#compile command for mycomp
assembler: $(OBJS)
	gcc $(CFLAGS) -g $^ -o $@

%.o: %.c
	gcc $(CFLAGS) -g -c $^ 

# cleanup clean:
clean:
	rm *.o

#debug building
#debug: mycomp.c complex.c parseinput.c complex.h  parseinput.h
#gcc $(CFLAGS) -g $^ -o $@ -lm

-include $(DEPS)
