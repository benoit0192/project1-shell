# example6.sh
# an example of valid shell script

# it supports conditional statements inline
if ls /bin then echo "an error occured" else echo "no error" fi

# on multiple lines?
if
    ../tools/errno 1
then
    echo "multiple lines"
    echo "no error"
else
    echo "an error occured"
    echo "in multiple lines"
fi

# and partial conditionial statments (no else)
if ../tools/errno 1 then echo "partial is valid" fi

# let's try something new without then
if ls /usr | wc -l else echo "partial is valid" fi

# programs running in backgroud are treated as if they returned 0
if ls /bin & then echo "not here" else echo "yeah" fi
