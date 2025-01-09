#!/bin/sh

trap 'rm -f file.txt; kill -KILL $pid 2>/dev/null' EXIT INT TERM HUP

scc-make -f - <<'EOF' 2>&1 &
file.txt:
	@touch $@
	@test -f $@
	@while : ; do sleep 1 ; done
EOF

pid=$!

for i in 1 2 3
do
	if test -f file.txt
	then
		kill $pid
		for i in 1 2 3
		do
			test -f file.txt || exit 0
			sleep 1
		done
		exit 1
	fi
	sleep 1			
done

exit 1
