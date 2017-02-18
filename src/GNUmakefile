
# Run this Makefile with 'gmake' instead of 'make' in Minix 3 (or $^ won't work)

LEX=lex
YACC=bison
CFLAGS=-g -Wall
CC=clang

SOURCES=main.c \
	    grammar.c \
	    scanner.c \
		input.c \
		autocomplete.c \
		execute.c \
		structures/statement.c \
		structures/sequence.c \
		structures/command.c \
		structures/assignment.c \
		structures/group.c

all:shell

shell: $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

grammar.c:grammar.y
	$(YACC) -t -d -o $@ --defines=grammar.tab.h $^

scanner.c:scanner.l
	$(LEX) -o $@ $^

tools/errno:tools/errno.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f grammar.c grammar.tab.h scanner.c shell tools/errno
