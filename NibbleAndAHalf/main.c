//
//  main.c
//  https://github.com/superwills/NibbleAndAHalf
//
//  See "base64.h" for compile-time settings & copyright notice.
//  
//  nibble&a half
//  OR PROJECT SEXTET STREAM
// A nibble is 4 bits. An octet (byte) is 8 bits, and a sextet is 6 bits,
// or a nibble and a half.

#include "base64.h"   // THIS IS ALL YOU NEED to use base64 and unbase64.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "testbase64.h"

int main( int argc, char** argv )
{
  printUnbase64();  // Generate the unbase64 conversion array
  
  testBase64String( "hi there aardvark!! @#$**&^)" );
  testBase64String( "" ); // sweet empty string test case
  
  testUnbase64WithBadAscii();
  testUnbase64WithBadLength();
  
  automatedTests();
  
  return 0;
}



