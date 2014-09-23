#include "proc.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

proc_t **proclist;
int      proclist_length = 0;

proclist_filter_func proclist_filter;

void proc_init(void) {
	proclist = readproctab(
		PROC_FILLCOM |
		PROC_FILLUSR |
		PROC_FILLGRP |
		PROC_FILLSTAT
	);
	for (proc_t **procp = proclist; *procp; procp++) {
		proclist_length++;
	}
}

void proc_cleanup(void) {
}

void proc_setfilter(proclist_filter_func func) {
	proclist_filter = func;
}

proc_t *proc_get_random(void) {
	if (!proclist_length) {
		return NULL;
	}
	proc_t *proc;
	do {
		proc = proclist[rand() % proclist_length];
	} while (!proclist_filter(proc));
	return proc;
}

int proc_kill(proc_t *proc) {
	// TODO
	return 0;
}
