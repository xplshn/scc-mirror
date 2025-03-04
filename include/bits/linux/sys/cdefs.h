#ifdef _NEED_MBSTATE_T
#ifndef _MBSTATE_T
typedef struct __mbstate_t {
	unsigned __opaque1, __opaque2;
} mbstate_t;
#define _MBSTATE_T
#endif
#endif
