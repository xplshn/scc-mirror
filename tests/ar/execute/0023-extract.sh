#!/bin/sh

set -e

tmp1=`mktemp`
tmp2=`mktemp`

trap "rm -f file* $tmp1 $tmp2" 0 2 3 15

############################################################################
#extract 1st member

cp master.a file.a
$EXEC scc ar -xv file.a file1

cat <<EOF > $tmp1
This is the first file,
and it should go in the
first position in the archive.
EOF

cmp file1 $tmp1
