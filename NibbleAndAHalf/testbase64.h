//
//  base64test.h
//  NibbleAndAHalf
//
//  Created by William Sherif on 4/17/13.
//  UNIT TESTS ONLY.
//  YOU DO NOT NEED THIS FILE WHEN USING "base64.h"
//
#ifndef BASE64TEST_H
#define BASE64TEST_H

#include "Timer.h"

// Function for automated testing of base64.h.  Also times.
int testbase64( const void* data, int dataLen );

void testunbase64withbadascii(void);

// -- other util --
// Prints the unb64 array in base64.h
void printUnbase64(void);

void failingTest(void);

void testUnbase64InvalidInput(void);

void automatedTests(void);

#endif
