#!/bin/bash

cal=10

gcc $1
if [[ $? -ne 0 ]] ; then 
	echo "grade: $cal"
	exit 1
fi

./a.out 
if [[ $? -ne 254 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
cal=$((cal+10))

./a.out dont_exist output.txt
if [[ $? -ne 253 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
cal=$((cal+15))

./a.out input.txt output.txt
if [[ $? -ne 0 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
cal=$((cal+15))

diff original.txt output.txt
if [[ $? -ne 0 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
cal=$((cal+50))

echo "=>grade: $cal"
