# example6.sh
# an example of valid shell script

# it can set environment variables
myvar="var \"content\""

# it can access environment variables
echo $myvar

# it can unset variables like this
myvar=
echo $myvar

# unexisting vars are considered empty (no error)
echo $idontexist
