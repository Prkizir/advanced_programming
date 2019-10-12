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

./a.out cha not_exist 
if [[ $? -ne 253 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
cal=$((cal+10))

./a.out cha dir1 -x
if [[ $? -ne 252 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
cal=$((cal+10))

./a.out cha dir1 > run1.txt
if [[ $? -ne 0 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
diff run1.txt nonrecursive.txt
if [[ $? -ne 0 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
cal=$((cal+30))
rm run1.txt

./a.out cha dir1 -r > run2.txt
if [[ $? -ne 0 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
diff run2.txt recursive.txt
if [[ $? -ne 0 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
cal=$((cal+30))
rm run2.txt

echo "=>grade: $cal"
