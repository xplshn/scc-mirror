#!/bin/sh

trap 'rm -f $tmp1 $tmp2 f f.?' EXIT INT TERM QUIT HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat >$tmp1 <<EOF
c99 -O  -o f f.c
EOF

cat >f.c <<'EOF'
int
main(void)
{
	return 0;
}
EOF

touch -d '1970-01-01 00:00:01' f.h
touch -d '1970-01-01 00:00:03' f
touch -d '1970-01-01 00:00:04' f.c

scc make -f- <<'EOF' > $tmp2  2>&1
f: f.h

f.h:
	touch $@
EOF

diff $tmp1 $tmp2
