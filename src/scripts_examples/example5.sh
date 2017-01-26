# example5.sh
# an example of valid shell script

# pipe has precedence over &
ls /bin | grep up | wc -l & sleep 3 & ls /usr | wc -l
