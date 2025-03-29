#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT TERM QUIT HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat >$tmp2 <<EOF
But before!
Hello World!
EOF

scc make -f- <<'EOF' > $tmp1 2>&1
STARGET = all

b.$(STARGET): b$(STARGET)
	@echo Hello World!

ball:
	@echo But before!
EOF

diff -u $tmp1 $tmp2
