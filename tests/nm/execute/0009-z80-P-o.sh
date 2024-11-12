#!/bin/sh

set -e

tmp1=`mktemp`
tmp2=`mktemp`
trap "rm -f $tmp1 $tmp2; exit" 0 2 3

scc nm -P -t o z80.out > $tmp1

cat <<! > $tmp2
.bss b 0000000000000000 0
.data d 0000000000000000 0
.text t 0000000000000000 0
averylongbss B 0000000000000001 0
averylongdata D 0000000000000001 0
averylongtext T 0000000000000001 0
bss1 B 0000000000000000 0
bss3 b 0000000000000002 0
bss4 C 0000000000000012 12
bss5 C 0000000000000022 22
data1 D 0000000000000000 0
data3 d 0000000000000002 0
data4 C 0000000000000012 12
data5 C 0000000000000022 22
text1 T 0000000000000000 0
text3 t 0000000000000002 0
text4 C 0000000000000012 12
text5 C 0000000000000022 22
text6 U
!

diff $tmp1 $tmp2
