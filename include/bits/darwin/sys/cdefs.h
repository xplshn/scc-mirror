#ifdef _NEED_MBSTATE_T
#ifndef _MBSTATE_T
typedef struct {
    unsigned char state[4];
    size_t count;
} mbstate_t;
#define _MBSTATE_T
#endif
#endif
