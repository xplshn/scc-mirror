#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat > $tmp1 <<EOF
/usr/local/include
EOF

scc make -f - <<'EOF' > $tmp2 2>&1
prefix = /usr/local
includedir = ${prefix}/include

all:
	@echo $(includedir)
EOF

diff $tmp1 $tmp2
