#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT TERM QUIT HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat > $tmp1 <<EOF
Hello World!
EOF

scc make -f- MK=test.mk hello <<'EOF' > $tmp2 2>&1
MK = fail.mk
include $(MK)
EOF
diff $tmp1 $tmp2
