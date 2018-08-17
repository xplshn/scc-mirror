#!/bin/sh

set -e
trap "rm -f $$.tmp" 0 2 3

(sed '/^#deps/q' deps.mk
for i in `find . -name '*.c'`
do
	file=`basename $i | sed 's/\.c$/.o/'`

	dir=`dirname $i |
	     sed -e 's,^\./,,' -e 's,^\.$,,' -e 's,...*[^/],&/,'`

	sed -n "/#include \"/ s,#include \"\(.*\)\",$dir$file: $dir\1,p" $i
done |
LC_ALL=C sort -s) > $$.tmp && mv $$.tmp deps.mk
