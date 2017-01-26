# example3.sh
# an example of valid shell script

# processes can pipe standard output from one to another
ls -l -a / | wc -l

ls /bin | grep up | wc -l
