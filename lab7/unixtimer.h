#ifndef _UNIXTIMER_H_
#define _UNIXTIMER_H_

/* for CPU time consumed by the process */
void   start_timer();
double cpu_seconds();

/* for real (i.e., wall clock) time */
void   start_clock();
double clock_seconds();

#endif
