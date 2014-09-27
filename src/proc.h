/*
 * Copyright (c) 2014 PolyFloyd
 */

#ifndef __PROC_H
#define __PROC_H

#include <proc/readproc.h>

#define PROC_ALL  0x0001
#define PROC_TEST 0x0002

typedef char (*proclist_filter_func)(proc_t*);

void    proc_init(void);
void    proc_cleanup(void);
void    proc_setfilter(proclist_filter_func);
proc_t *proc_get_random(void);
int     proc_kill(proc_t *proc, int sign);

#endif
