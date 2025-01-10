#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT TERM HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat > $tmp1 <<EOF
make: error: <stdin>:5: missing target
EOF

scc make -f - <<'EOF' > $tmp2 2>&1
FLAGS=Hello $(EFLAGS)
all: $(DIRS)
	@echo Hello World

$(DIRS): FORCE
	@echo Bye

FORCE:
EOF

diff $tmp1 $tmp2
