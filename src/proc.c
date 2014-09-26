#include "proc.h"
#define __USE_POSIX
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

proc_t **proclist;
int      proclist_length = 0;

proclist_filter_func proclist_filter;

void proc_init(void) {
	proc_t **procs = readproctab(
		PROC_FILLCOM |
		PROC_FILLUSR |
		PROC_FILLGRP |
		PROC_FILLSTAT
	);
	for (proc_t **procp = procs; *procp; procp++) {
		proclist_length++;
	}
	proclist = malloc(proclist_length * sizeof(proc_t*));
	int i = 0;
	for (proc_t **procp = procs; *procp; procp++) {
		if (proclist_filter(*procp)) {
			proclist[i++] = *procp;
		} else {
			proclist_length--;
		}
	}
	proclist = realloc(proclist, proclist_length * sizeof(proc_t*));
}

void proc_cleanup(void) {
	free(proclist);
}

void proc_setfilter(proclist_filter_func func) {
	proclist_filter = func;
}

proc_t *proc_get_random(void) {
	int remaining = 0;
	for (int i = 0; i < proclist_length; i++) {
		remaining += proclist[i] ? 1 : 0;
	}
	if (!remaining) {
		return NULL;
	}
	proc_t *proc;
	do {
		proc = proclist[rand() % proclist_length];
	} while (!proc);
	return proc;
}

int proc_kill(proc_t *proc, int sign) {
	for (int i = 0; i < proclist_length; i++) {
		if (proc == proclist[i]) {
			proclist[i] = NULL;
			break;
		}
	}
	return kill(proc->tgid, sign);
}
