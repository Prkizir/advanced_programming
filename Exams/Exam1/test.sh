#!/bin/bash

cal=10

gcc $1
if [[ $? -ne 0 ]] ; then 
	echo "grade: $cal"
	exit 1
fi

./a.out dont_care dont_care
if [[ $? -ne 255 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
cal=$((cal+10))

./a.out no_existe
if [[ $? -ne 254 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
cal=$((cal+10))

./a.out $1
if [[ $? -ne 254 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
cal=$((cal+10))

./a.out dir1
if [[ $? -ne 0 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
cal=$((cal+30))

./a.out dir1 > resultado.txt
diff salida.txt resultado.txt
if [[ $? -ne 0 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
cal=$((cal+30))

echo "=>grade: $cal"

rm a.out resultado.txt

