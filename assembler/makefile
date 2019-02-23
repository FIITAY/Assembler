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

.PHONY: tests
tests: assembler
	./run_tests.sh

-include $(DEPS)
