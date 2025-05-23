#!/bin/sh

set -e

tmp1=`mktemp`
tmp2=`mktemp`

trap "rm -f file.a $tmp1 $tmp2" 0 2 3 15

############################################################################
#print 1st member

cp master.a file.a

$EXEC scc ar -p file.a file1 > $tmp1

cat <<! > $tmp2
This is the first file,
and it should go in the
first position in the archive.
!

cmp $tmp1 $tmp2
