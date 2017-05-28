#ifndef APPLICATION_H
#define APPLICATION_H

#include <rtthread.h>
#include <rthw.h>

enum EBOOL{FALSE, TRUE};

#ifndef NULL
#define NULL 0
#endif

#define RT_THREAD_PRI_HIGH RT_THREAD_PRIORITY_MAX /3
#define RT_THREAD_PRI_MID RT_THREAD_PRIORITY_MAX / 3 + 4
#define RT_THREAD_PRI_LOW RT_THREAD_PRIORITY_MAX / 3 + 8

#endif