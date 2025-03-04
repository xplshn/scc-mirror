#ifdef _NEED_MBSTATE_T
#ifndef _MBSTATE_T
typedef union {
	char __mbstate8[128];
} mbstate_t;
#define _MBSTATE_T
#endif
#endif
