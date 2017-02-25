# This project uses a GNU makefile
# The make command on Minix 3 is BSD makefile (not entirely compatible
# with GNU makefile syntax)
# The following rule automatically use gmake whenever you call make
# You can install gmake with 'pkgin install gmake'

USEGNU=gmake $*
all:
    @$(USEGNU)
.DEFAULT:
    @$(USEGNU)
