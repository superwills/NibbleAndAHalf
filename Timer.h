#ifndef TIMER_H
#define TIMER_H

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif

// These timing functions are always available
typedef struct _CTimer
{
  #ifdef _WIN32
  LARGE_INTEGER startTime ;
  double fFreq ;
  #else
  struct timeval startTime ;
  #endif
} CTimer ;

void CTimerReset( CTimer* ctimer );

void CTimerInit( CTimer* ctimer );

// Gets the most up to date time.
double CTimerGetTime( CTimer* ctimer );

#endif
