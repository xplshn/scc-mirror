#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT HUP TERM

tmp1=tmp1.$$
tmp2=tmp2.$$

scc make -ftest.mk print-cc print-makeflags > $tmp1 2>&1

cat <<EOF > $tmp2
c99

EOF

diff $tmp1 $tmp2
