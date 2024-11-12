#!/bin/sh

trap 'rm -f $tmp1 $tmp2' EXIT INT TERM QUIT HUP

tmp1=tmp1.$$
tmp2=tmp2.$$

cat > $tmp1 <<EOF
file1.o file2.o
all
alp
all.o
file1.c file2.c
file1 file2
file1.e file2.e
file1.c file2.c
all
EOF

scc make -f- <<'EOF' > $tmp2 2>&1
O = o
SRC = file1.c  file2.c
OBJ = $(SRC:.c=.$O)
O = e
C = c

all:
	@echo $(SRC:.c=.o)
	@echo $(@)
	@echo $(@:l=p)
	@echo $(@:=.o)
	@echo $(OBJ:.$O=.$C)
	@echo $(OBJ:.$O=)
	@echo $(OBJ)
	@echo $(SRC)
	@echo $(@:=)
EOF
diff $tmp1 $tmp2

scc make -f- <<'EOF' > $tmp2 2>&1
O = o
SRC = file1.c  file2.c
OBJ = ${SRC:.c=.$O}
O = e
C = c

all:
	@echo ${SRC:.c=.o}
	@echo ${@}
	@echo ${@:l=p}
	@echo ${@:=.o}
	@echo ${OBJ:.$O=.$C}
	@echo ${OBJ:.$O=}
	@echo ${OBJ}
	@echo ${SRC}
	@echo ${@:=}
EOF
diff $tmp1 $tmp2
