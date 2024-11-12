#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT QUIT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$
make=`command -v scc-make`

echo $make > $tmp2

$make -f test.mk print-make > $tmp1 2>&1

diff $tmp1 $tmp2
