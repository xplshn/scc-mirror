#!/bin/sh

set -e
cpu=${1?'missed cpu argument'}
exec >>test.log
exec 2>&1

tmp1=tmp1.$$
tmp2=tmp2.$$
file=$cpu.s

trap "rm -f a.out $tmp1 $tmp2" 0 2 3

scc as -m $cpu $file

sed -n '/^\#/ ! {
	/\#/ ! d
	s%.*#%%
	s%^[ 	]*%%
	s%[ 	]*$%%
	/^$/d
	s%[ 	][ 	]*%\
%g
	p
}' $file |
nl -b a > $tmp1


scc dump |
sed -n '/^data:/,$ {
	/^data:/ ! {
		s%.*:%%
		s%^[ 	]*%%
		s%[ 	]*$%%
		/^$/d
		s%[ 	][ 	]*%\
%g
		p
	}
}' |
nl -b a > $tmp2

echo diff
diff -u $tmp1 $tmp2
