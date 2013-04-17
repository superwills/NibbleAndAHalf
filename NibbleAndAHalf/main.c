//
//  main.cpp
//  NibbleAndAHalf
//
//  See "base64.h" for compile-time settings & copyright notice.
//  
//  nibble&a half
//  OR PROJECT SEXTET STREAM 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Timer.h"
#include "base64.h"   // THIS IS ALL YOU NEED to use base64 and unbase64.

// Verbosely shows the base64 test data.
// Don't uncomment this unless you are testing small data sizes
// Affects testbase64 function in "testbase64.h" only.
//
//     #define BASE64TESTSHOWDATA
#include "testbase64.h"

// Constant used IN THIS FILE:
//int BASE64TESTMAXDATALEN=1<<4 ; // test up to 16 bytes of random data. tests can go much bigger,
                                // BUT YOU SHOULD TURN BASE64TESTSHOWDATA OFF IF YOU DO THAT.

int BASE64TESTMAXDATALEN=1<<27; // Tests up to 128 MB

int main( int argc, char** argv )
{
  //printUnbase64() ;  return 1 ; // Show me the unbase64 conversion array
  int allOk=1;
  int testDatLen=1;
  int i ;
  
  // WRITE YOUR OWN TEST
  const char * str = "hi there aardvark!! @#$**&^)" ;
  testbase64( str, (int)strlen(str) ) ;
  testunbase64withbaddat();
  
  ///*
  // AUTOMATED TESTS
  //srand( 220 ); // want same sequences
  srand( (unsigned int)time(0) ) ;
  
  for( testDatLen = 1 ; testDatLen <= BASE64TESTMAXDATALEN ; testDatLen <<= 1 )
  {
    unsigned char *dat = (unsigned char*)malloc( testDatLen ) ;
    for( i = 0 ; i < testDatLen ; i++ )
      dat[i]=rand(); // make new random data
      
    allOk |= testbase64( dat, testDatLen ) ;
    
    free( dat ) ;
    
    if( !allOk )  break ;
  }
  
  if( allOk )  puts( "-- ALL TESTS COMPLETED SUCCESSFULLY --" ) ;
  else puts( "ERROR: At least one test failed. You should check it out." ) ;
  //*/
  
  return 0;
}



