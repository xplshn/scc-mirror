#!/bin/sh

set -e

tmp1=`mktemp`
tmp2=`mktemp`

trap "rm -f file.a f1 f2 f3 $tmp1 $tmp2" 0 2 3 15

###########################################################################
#Append generated files at once to an empty archive

echo First > file1
echo Second > file2
echo Third > file3

rm -f file.a
$EXEC scc ar -qv file.a file1 file2 file3

$EXEC scc ar -t file.a > $tmp1

cat <<EOF > $tmp2
file1
file2
file3
EOF

cmp $tmp1 $tmp2

$EXEC scc ar -p file.a > $tmp1

cat <<EOF > $tmp2
First
Second
Third
EOF

cmp $tmp1 $tmp2
