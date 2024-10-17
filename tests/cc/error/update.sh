#!/bin/sh

for i
do
	(echo '/^PATTERN/+;/^\./-c'
	 scc $CFLAGS -W -c $i 2>&1
	 printf ".\nw\n"
	 echo w) |
	ed -s $i
done
