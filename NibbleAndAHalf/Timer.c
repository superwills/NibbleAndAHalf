//
//  Timer.c
//  NibbleAndAHalf
//
//  Created by William Sherif on 2022-02-01.
//  Copyright © 2022 William Sherif. All rights reserved.
//

#include "Timer.h"

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

