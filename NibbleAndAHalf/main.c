//
//  main.c
//  https://github.com/superwills/NibbleAndAHalf
//
//  See "base64.h" for compile-time settings & copyright notice.
//  
//  nibble&a half
//  OR PROJECT SEXTET STREAM 

#include "base64.h"   // THIS IS ALL YOU NEED to use base64 and unbase64.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int BASE64TESTSHOWDATA;

#include "testbase64.h"

int main( int argc, char** argv )
{
  //printUnbase64();  // Generate the unbase64 conversion array
  // Want to see the data output for small data sizes
  BASE64TESTSHOWDATA = 1;
  puts("Test against a custom string.");
  const char * str = "hi there aardvark!! @#$**&^)" ;
  testbase64( str, (int)strlen(str)+1 ) ;  // Notice use of length of strlen(str)+1 to include NULL in the base64 encoding
  //testbase64( "", 0 ) ; // sweet empty string test case
  
  //puts("Bad ascii tests.");
  //testunbase64withbadascii();
  
  //BASE64TESTSHOWDATA = 0; // Don't show the output for large data
  //puts("Automated tests.");
  //automatedTests();
  
  return 0;
}



