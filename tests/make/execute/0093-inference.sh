#!/bin/sh

trap 'rm -f f.? $tmp1 $tmp2 $tmp3 f.?' EXIT INT TERM QUIT HUP

tmp1=tmp1.$$
tmp2=tmp2.$$
tmp3=tmp3.$$

cat >$tmp1<<EOF
c99 -O -c f.c
EOF

cat >$tmp2 <<'EOF'
f.o: f.h

all: f.o
EOF

(touch f.c f.h
scc make -f $tmp2
touch f.c
scc make -f $tmp2) > $tmp3

diff $tmp1 $tmp3
