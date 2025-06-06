#!/bin/sh

file=${1?' empty input file'}
tmp1=`mktemp`
tmp2=`mktemp`
trap "rm -f *.o  $tmp1 $tmp2" EXIT INT QUIT TERM
ulimit -c 0
rm -f test.log

while read i state
do
	rm -f *.o $i $tmp1 $tmp2

	(echo $i
	 $CC $CFLAGS -o $i $i.c
	 echo '/^output:$/+;/^end:$/-'w $tmp1 | ed -s $i.c
	 ./$i > $tmp2 2>&1
	 diff -u $tmp1 $tmp2) >> test.log 2>&1 &&

	printf '[PASS]' || printf '[FAIL]'
        printf '\t%s\t%s\n' $i $state
done < $file
