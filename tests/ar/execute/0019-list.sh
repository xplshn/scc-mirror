#!/bin/sh

set -e

tmp1=`mktemp`
tmp2=`mktemp`

trap "rm -f file.a $tmp1 $tmp2" 0 2 3 15

############################################################################
#print 3rd member

cp master.a file.a
$EXEC scc ar -t file.a file3 > $tmp1

cat <<! > $tmp2
file3
!

cmp $tmp1 $tmp2
