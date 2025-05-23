#!/bin/sh


set -e

trap "rm -f file.a" 0 2 3 15

############################################################################
#no members
cp master.a file.a

last=`ls -l file.a | awk '{print $6,$7,$8}'`

if ! $EXEC scc ar -dv file.a
then
	echo ar returned with error when no members
	exit 1
fi

now=`ls -l file.a | awk '{print $6,$7,$8}'`
if test "$now" != "$last"
then
	echo empty ar -d modified the archive >&2
	exit 1
fi
