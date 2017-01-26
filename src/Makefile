LEX=lex
YACC=bison
CFLAGS=-Wall
CC=gcc

all:shell tools/errno

shell:grammar.c scanner.c
	$(CC) $(CFLAGS) -o $@ $^

grammar.c:grammar.y
	$(YACC) -t -d -o $@ --defines=grammar.tab.h $^

%.c:%.l
	$(LEX) -o $@ $^

tools/errno:tools/errno.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f grammar.c grammar.tab.h scanner.c shell *~ tools/errno
