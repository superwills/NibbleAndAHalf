//
//  Timer.h
//  NibbleAndAHalf
//

#ifndef TIMER_H
#define TIMER_H

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif

#ifdef __cplusplus
// TIMER class, only available if C++ available
class Timer
{
  #ifdef _WIN32
  LARGE_INTEGER startTime ;
  double fFreq ;
  #else
  timeval startTime ;
  #endif
  
public:
  Timer() {
    #ifdef _WIN32
    LARGE_INTEGER freq ;
    QueryPerformanceFrequency( &freq ) ;
    fFreq = (double)freq.QuadPart ;
    #else
    gettimeofday( &startTime, NULL ) ;
    #endif
    reset();
  }

  void reset() {
    #ifdef _WIN32
    QueryPerformanceCounter( &startTime ) ;
    #else
    gettimeofday( &startTime, NULL ) ;
    #endif
  }

  // Gets the most up to date time.
  double getTime() const {
    #ifdef _WIN32
    LARGE_INTEGER endTime ;
    QueryPerformanceCounter( &endTime ) ;
    return ( endTime.QuadPart - startTime.QuadPart ) / fFreq ; // as double
    #else
    timeval endTime ;
    gettimeofday( &endTime, NULL ) ;
    return (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec)/1e6 ;
    #endif
  }
} ;
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

void CTimerReset( CTimer* ctimer ) {
  #ifdef _WIN32
  QueryPerformanceCounter( &ctimer->startTime ) ;
  #else
  gettimeofday( &ctimer->startTime, NULL ) ;
  #endif
}

void CTimerInit( CTimer* ctimer ) {
  #ifdef _WIN32
  LARGE_INTEGER freq ;
  QueryPerformanceFrequency( &freq ) ;
  ctimer->fFreq = (double)freq.QuadPart ;
  #else
  gettimeofday( &ctimer->startTime, NULL ) ;
  #endif
  CTimerReset( ctimer );
}

// Gets the most up to date time.
double CTimerGetTime( CTimer* ctimer ) {
  #ifdef _WIN32
  LARGE_INTEGER endTime ;
  QueryPerformanceCounter( &endTime ) ;
  return ( endTime.QuadPart - ctimer->startTime.QuadPart ) / ctimer->fFreq ; // as double
  #else
  struct timeval endTime ;
  gettimeofday( &endTime, NULL ) ;
  return (endTime.tv_sec - ctimer->startTime.tv_sec) + (endTime.tv_usec - ctimer->startTime.tv_usec)/1e6 ;
  #endif
}


#endif
