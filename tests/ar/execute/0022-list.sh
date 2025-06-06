#!/bin/sh

set -e

tmp1=`mktemp`
tmp2=`mktemp`

trap "rm -f file.a $tmp1 $tmp2" 0 2 3 15

############################################################################
#list 1st member

cp master.a file.a

$EXEC scc ar -t file.a file1 > $tmp1

cat <<! > $tmp2
file1
!

cmp $tmp1 $tmp2

############################################################################
#print 3rd member

$EXEC scc ar -t file.a file3 > $tmp1

cat <<! > $tmp2
file3
!

cmp $tmp1 $tmp2

############################################################################
#print 2nd member with verbose

$EXEC scc ar -tv file.a file2 >$tmp1

cat <<! > $tmp2
rw-r--r-- `id -u`/`id -g`	Tue Jan  1 00:00:00 1980 file2
!

cmp $tmp1 $tmp2

############################################################################
#print all members

$EXEC scc ar -t file.a file1 file2 file3 >$tmp1

cat <<! > $tmp2
file1
file2
file3
!

cmp $tmp1 $tmp2

#and now with no members in command line

$EXEC scc ar -t file.a > $tmp1

cmp $tmp1 $tmp2
