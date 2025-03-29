#!/bin/sh

trap 'rm -f $tmp1 $tmp2 $tmp3' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$
tmp3=tmp3.$$

touch $tmp3

echo Hello World! > $tmp2

scc make -f- hello <<EOF > $tmp1 2>&1
FILE = test.mk
F = $tmp3

include \$(FILE)
include \$(F)
EOF

diff $tmp1 $tmp2
