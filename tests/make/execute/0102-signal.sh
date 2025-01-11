#!/bin/sh

cleanup()
{
	rm -rf adir
	kill -KILL $pid 2>/dev/null
	if test $1 -ne 0
	then
		kill -KILL $$
	fi
}

rm -rf adir
trap 'cleanup 0' EXIT
trap 'cleanup 1' INT TERM HUP

scc make -f - <<'EOF' &
adir:
	@mkdir $@
	@while : ; do sleep 1 ; done
EOF
pid=$!

sleep 10 && echo timeout >&2 && kill $$ 2>/dev/null &
timer=$!

while :
do
	if test -d adir
	then
		kill $pid
		wait $pid
		kill $timer
		break
	fi
done

test -d adir
