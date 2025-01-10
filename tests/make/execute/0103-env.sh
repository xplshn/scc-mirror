#!/bin/sh

export EFLAGS=World

trap 'rm -f $tmp1 $tmp2' EXIT INT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat > $tmp1 <<EOF
Hello World
EOF

scc-make -f - <<'EOF' > $tmp2 2>&1
FLAGS=Hello $(EFLAGS)
all:
	@echo $(FLAGS)
EOF

diff $tmp1 $tmp2
