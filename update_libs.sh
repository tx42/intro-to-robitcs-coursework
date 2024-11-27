#!/bin/bash
# algorithm folder supplied as a parameter

if [ -z "$1" ]
then
    echo "Error: algorithm path required"
    exit 1
fi
# get a list of all current libs
for lib in `find lib -name '*.h' -or -name '*.cpp' -maxdepth 2`
do
    # check if dependencies exsist
    for instance in `find $1 -name '*.h' -or -name '*.cpp'`
    do
        if [[ `basename $instance` != `basename $lib` ]]
        then
            continue
        fi

        cp $lib $instance
    done
done