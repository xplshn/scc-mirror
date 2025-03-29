#!/bin/sh

if test -z $SCCPREFIX
then
	SCCPREFIX=`dirname $0`/..
	export SCCPREFIX
fi

if test -z $SCCLIBPREFIX
then
	SCCLIBPREFIX=`dirname $0`/..
	export SCCLIBPREFIX
fi

case $1  in
cc|cpp|as|ar|addr2line|ld|make|nm|objcopy|dump|size|strip)
	tool=$1
	shift
	exec $SCCPREFIX/bin/scc-$tool $@
	;;
*)
	exec $SCCPREFIX/bin/scc-cc $@
	;;
esac
