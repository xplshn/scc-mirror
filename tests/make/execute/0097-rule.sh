#!/bin/sh

trap 'rm -f f1.? $tmp1 $tmp2' EXIT INT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat > $tmp1 <<EOF
touch f1.c
touch f1.o
EOF

scc make -f- <<'EOF' > $tmp2 2>&1
all: f1.o

f1.o: f1.c
	touch $@

f1.c:
	touch $@
EOF

diff $tmp1 $tmp2
