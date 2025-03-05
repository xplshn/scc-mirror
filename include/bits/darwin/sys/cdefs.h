#ifdef _NEED_MBSTATE_T
#ifndef _MBSTATE_T
typedef struct {
    unsigned char state[MB_CUR_MAX];
    size_t count;
} mbstate_t;
#define _MBSTATE_T
#endif
#endif
