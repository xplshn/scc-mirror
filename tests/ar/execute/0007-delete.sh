#!/bin/sh


set -e

trap "rm -f file.a" 0 2 3 15

############################################################################
#delete one member

cp master.a file.a

$EXEC scc ar -dv file.a file2

if $EXEC scc ar -tv file.a file2
then
	echo file-2 was not deleted >&2
	exit 1
fi
