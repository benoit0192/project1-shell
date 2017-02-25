# example7.sh
# an example of valid shell script

# commands can be grouped
msg="Hello World!" ; (ls -la | wc -l ; uname -a) & echo $msg; sleep 1
