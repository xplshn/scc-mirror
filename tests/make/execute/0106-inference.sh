#!/bin/sh

trap 'rm -f f.? $tmp1 $tmp2 $tmp3 f.?' EXIT INT TERM QUIT HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat >$tmp1<<EOF
c99 -O -c f.c
EOF

touch -d '1970-01-01 00:00:01' f.c
touch -d '1970-01-01 00:00:02' f.o
touch -d '1970-01-01 00:00:03' f.h
scc make -f - <<'EOF' > $tmp2 2>&1
f.o: f.h

all: f.o
EOF

diff $tmp1 $tmp2
