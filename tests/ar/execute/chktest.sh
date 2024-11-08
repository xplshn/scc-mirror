#!/bin/sh

trap "rm -rf file*" EXIT HUP INT QUIT TERM
ulimit -c 0
rm -f test.log
rm -rf file*

export TZ=${TZ:-UTC}

for i in *-*.sh
do
	printf "Test: %s\n\n" $i >> test.log
	(./$i >> test.log 2>&1 && printf '[PASS]\t' || printf '[FAIL]\t'
	echo "$i") | tee -a test.log
done
