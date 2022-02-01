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

// Only want to see the data output for small data sizes
int BASE64TESTSHOWDATA = 0;

int BASE64TESTMAXDATALEN=1<<27; // Tests up to 128 MB

#include "testbase64.h"

void automatedTests()
{
  int allOk=1;
  int testDatLen=1;
  int i ;
  
  //srand( 220 ); // want same sequences
  srand( (unsigned int)time(0) ) ;
  
  for( testDatLen = 1 ; testDatLen <= BASE64TESTMAXDATALEN ; testDatLen <<= 1 )
  {
    unsigned char *dat = (unsigned char*)malloc( testDatLen ) ;
    for( i = 0 ; i < testDatLen ; i++ )
      dat[i]=rand(); // make new random data
      
    allOk &= testbase64( dat, testDatLen ) ;
    
    free( dat ) ;
    
    if( !allOk )  break ;
  }
  
  if( allOk )  puts( "-- ALL TESTS COMPLETED SUCCESSFULLY --" ) ;
  else puts( "ERROR: At least one test failed. You should check it out." ) ;
}

int main( int argc, char** argv )
{
  //printUnbase64();  // Generate the unbase64 conversion array
  
  BASE64TESTSHOWDATA = 1;
  puts("Test against a custom string.");
  const char * str = "hi there aardvark!! @#$**&^)" ;
  testbase64( str, (int)strlen(str)+1 ) ;  // length of strlen(str)+1 to include NULL in the base64 encoding
  //testbase64( "", 0 ) ; // sweet empty string test case
  
  //puts("Bad ascii tests.");
  //testunbase64withbadascii();
  
  BASE64TESTSHOWDATA = 0; // Don't show the output for large data
  //puts("Automated tests.");
  //automatedTests();
  
  return 0;
}



