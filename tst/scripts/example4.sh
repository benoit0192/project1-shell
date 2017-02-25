# example4.sh
# an example of valid shell script

# processes can run in parallel
# exit code is the one of the last command
ls /bin & ls /usr

# but all processes can also run in parallel
# exit code of this command line is considered to be always 0 since we don't wait for termination
ls /bin & ls /usr &
