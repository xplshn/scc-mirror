#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT TERM QUIT HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat > $tmp1 <<EOF
MACRO=hello
EOF


scc make -f- <<'EOF' > $tmp2 2>&1
MACRO = $$(echo hello)

all:
	@echo MACRO=$(MACRO)
EOF

diff $tmp1 $tmp2
