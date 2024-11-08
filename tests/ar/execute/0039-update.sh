#!/bin/sh

set -e

tmp1=`mktemp`
tmp2=`mktemp`

trap "rm -f file* $tmp1 $tmp2" 0 2 3 15

############################################################################
#Update one member that already exist

echo First > file1

cp master.a file.a
sleep 1
touch file1
$EXEC $AR -ruv file.a file1

$EXEC $AR -p file.a file1 > $tmp1

cat <<EOF > $tmp2
First
EOF

cmp $tmp1 $tmp2

echo Second > file1
touch -t 197001010000 file.1
$EXEC $AR -ruv file.a file1

$EXEC $AR -p file.a file1 > $tmp1

cat <<EOF > $tmp2
First
EOF

cmp $tmp1 $tmp2
