#include <scc/scc.h>

#include "../cc2.h"


Type int8type = {
	.flags  = SIGNF | INTF,
	.size   = 1,
	.align  = 1
};

Type int16type = {
	.flags  = SIGNF | INTF,
	.size   = 2,
	.align  = 1
};

Type int32type = {
	.flags  = SIGNF | INTF,
	.size   = 4,
	.align  = 1
};

Type int64type = {
	.flags  = SIGNF | INTF,
	.size   = 8,
	.align  = 1
};

Type uint8type = {
	.flags  = INTF,
	.size   = 1,
	.align  = 1
};

Type uint16type = {
	.flags  = INTF,
	.size   = 2,
	.align  = 1
};

Type uint32type = {
	.flags  = INTF,
	.size   = 4,
	.align  = 1
};

Type uint64type = {
	.flags  = INTF,
	.size   = 8,
	.align  = 1
};

Type ptrtype = {
	.flags  = INTF,
	.size   = 2,
	.align  = 1
};

Type booltype = {
	.flags  = INTF,
	.size   = 1,
	.align  = 1
};

Type float32type = {
	.flags  = FLOATF,
	.size   = 4,
	.align  = 1
};

Type float64type = {
	.flags  = FLOATF,
	.size   = 4,
	.align  = 1
};

Type float80type = {
	.flags  = FLOATF,
	.size   = 4,
	.align  = 1
};

Type voidtype = {
	.size = 0,
	.align = 0
};

Type arg_type = {
        .size = 2,
        .align = 1
};
