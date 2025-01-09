#!/bin/sh

trap "rm -f f1.?" EXIT INT TERM HUP

scc-make -df- <<'EOF'
all: f1.o

f1.o: f1.c
	touch $@

f1.c:
	touch $@
