if [ "$1" == "1" ]; then
    cat testfile1
elif [ "$1" == "2" ]; then
    cat testfile2
else
    echo "pass a proper argument"
fi
