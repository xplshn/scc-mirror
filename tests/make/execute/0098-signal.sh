#!/bin/sh

cleanup()
{
	rm -f test.txt
	kill -KILL $pid 2>/dev/null
	if test $1 -ne 0
	then
		kill -KILL $$
	fi
}

rm -f file.txt
trap 'cleanup 0' EXIT
trap 'cleanup 1' INT TERM HUP

scc-make -f - <<'EOF' 2>&1 &
file.txt:
	@touch $@
	@while : ; do sleep 1 ; done
EOF

pid=$!

sleep 10 && echo timeout >&2 && kill $$ 2>/dev/null &

while :
do
	if test -f file.txt
	then
		kill $pid 2>/dev/null
		wait $pid
		break
	fi
done

! test -f file.txt
