#!/bin/bash

cal=10

gcc $1 -lpthread
if [[ $? -ne 0 ]] ; then 
	echo "grade: $cal"
	exit 1
fi

./a.out
if [[ $? -ne 254 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
cal=$((cal+5))

./a.out -q nada -b b1181.txt -t 1
if [[ $? -ne 253 ]] ; then 
	echo "grade: $cal"
	exit 1
fi

./a.out -a a1181.txt -q nada -t 1
if [[ $? -ne 252 ]] ; then 
	echo "grade: $cal"
	exit 1
fi

./a.out -a a1181.txt -b b1181.txt -q nada
if [[ $? -ne 251 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
cal=$((cal+10))

./a.out -a noexiste.txt -b b1181.txt -t 1
if [[ $? -ne 250 ]] ; then 
	echo "grade: $cal"
	exit 1
fi

./a.out -a a1181.txt -b noexiste.txt -t 1
if [[ $? -ne 249 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
cal=$((cal+5))

./a.out -a a1181.txt -b b1181.txt -t -2
if [[ $? -ne 248 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
cal=$((cal+5))

./a.out -a a1181.txt -b c1143.txt -t 1
if [[ $? -ne 247 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
cal=$((cal+5))

./a.out -a a1181.txt -b b1181.txt -t 2 > test.txt
diff resultado.txt test.txt
if [[ $? -ne 0 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
cal=$((cal+50))
rm test.txt a.out

echo "=>grade: $cal"
echo "los puntos restantes se evalUan al revisar los comentarios"

