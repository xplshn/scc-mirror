#!/bin/sh

set -e

tmp1=`mktemp`
tmp2=`mktemp`

trap "rm -f file* $tmp1 $tmp2" 0 2 3 15

############################################################################
#Update one member that exists and using -a

echo First > file1

cp master.a file.a
$EXEC $AR -rv -a file1 file.a file1

$EXEC $AR -p file.a file1 > $tmp1

cat <<EOF > $tmp2
First
EOF

cmp $tmp1 $tmp2

$EXEC $AR -t file.a > $tmp1

cat <<EOF > $tmp2
file1
file2
file3
EOF

cmp $tmp1 $tmp2
