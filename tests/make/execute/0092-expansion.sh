#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT TERM QUIT HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat >$tmp2 <<EOF
yeah!
done
EOF

scc make -f- <<'EOF' > $tmp1 2>&1
VAR =\
        all\
        f1\
        f2\

OBJS = f.$O

O = e

$(VAR) all : $(OBJS)
	@echo done

f.$O:
	@echo yeah!
EOF

diff $tmp1 $tmp2
