#!/bin/sh

set -e

tmp1=`mktemp`
tmp2=`mktemp`

trap "rm -f file.a $tmp1 $tmp2" 0 2 3 15

############################################################################
#list 1st member

cp master.a file.a

$EXEC $AR -t file.a file1 > $tmp1

cat <<! > $tmp2
file1
!

cmp $tmp1 $tmp2
