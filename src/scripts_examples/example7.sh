# example7.sh
# an example of valid shell script

# it can set environment variables
myvar="var \"content\""

# it can access environment variables
echo $myvar

# unexisting vars are considered empty (no error)
echo $idontexist

# some vars are special
#echo $PWD      # current working directory, as set by cd builtin
echo $PROMPT   # the prompt invite (just text for now, but some special notations will be expanded in the future e.g. username, host, working directory...)
echo $?        # last return code
