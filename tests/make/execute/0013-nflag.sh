#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT QUIT INT HUP TERM

tmp1=tmp1.$$
tmp2=tmp2.$$

cat <<EOF > $tmp2
echo -n
no-valid-program
correct
EOF

(set -e
 MAKEFLAGS=-n scc make -f test.mk print-makeflags error
 echo correct) > $tmp1 2>&1

diff $tmp1 $tmp2 
