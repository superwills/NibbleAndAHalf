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
int testbase64( const void* data, int dataLen )
{
  // main ptrs
  unsigned char* binaryPtr = (unsigned char*)data ;
  char* base64Ascii ;
  unsigned char* recoveredData ;
  
  int outcome=1;
  int base64AsciiLen, recoveredLen ;
  int i ; //compare loop counter
  CTimer t; // for timing runs
  
  printf( "Base64 test with " ) ;
  if( dataLen < 1<<10 )  printf( "%d Bytes", dataLen ) ;
  else if( dataLen < 1<<20 )  printf( "%d KB", dataLen >> 10 ) ;
  else if( dataLen < 1<<30 )  printf( "%d MB", dataLen >> 20 ) ;
  else  printf( "%d GB", dataLen >> 30 ) ;
  printf( " of data\n" ) ;
  
  CTimerInit( &t ) ;
  base64Ascii = base64( data, dataLen, &base64AsciiLen ) ;
  if( !base64Ascii )  return 0 ; //memory failure
  printf( "base64 %f seconds\n", CTimerGetTime( &t ) ) ;
  
  CTimerReset( &t ) ;
  if( base64integrity( base64Ascii, base64AsciiLen ) ) // Check the integrity of the base64'd string
    puts( "All base64 encoded data are valid base64 alphabet characters" ) ;
  else
    puts( "ERROR: Bad base64 characters detected" ) ;
  printf( "base64 integrity check %f seconds\n", CTimerGetTime( &t ) ) ;
  
  CTimerReset( &t ) ;
  recoveredData = unbase64( base64Ascii, base64AsciiLen, &recoveredLen ) ;
  if( !recoveredData )  return 0 ; //memory failure, or invalid base64 data
  printf( "unbase64 %f seconds\n", CTimerGetTime( &t ) ) ;
  
  if( BASE64TESTSHOWDATA )
  {
    puts( "Original text:" ) ;
    puts( (char*)data ) ;
    puts( "--------------------" ) ;
    puts( "Base64'd ascii text:" ) ;
    puts( base64Ascii );
    puts( "--------------------" ) ;
    puts( "Unbase64'd recovered data:" ) ;
    puts( recoveredData ) ;
    puts( "--------------------" ) ;
  }
  
  printf( "base64: %d bytes => %d bytes => %d bytes\n", dataLen, base64AsciiLen, recoveredLen ) ;
  puts( "Checking.." ) ;
  if( dataLen != recoveredLen )
  {
    puts( "ERROR: length( unbase64( base64( data ) ) ) != length( data )" ) ;
    puts( "TEST FAILED" ) ;
    outcome = 0 ;
  }
  // Data is the exact same len. Good.
  else for( i = 0 ; i < dataLen ; i++ ) // good ol' else for
  {
    if( BASE64TESTSHOWDATA )
      printf( "\n%4d  | %3d | %3d |", i, binaryPtr[i], recoveredData[i] ) ;
    
    if( binaryPtr[i] != recoveredData[i] )
    {
      printf( " X" ) ; //\nERROR: byte @ %d != original data: (%d != %d)\n", i, data[i], recoveredData[i] );
      outcome = 0 ;
    } 
  }
  
  free( base64Ascii ) ;
  free( recoveredData ) ;
  if( outcome )
    puts( "\n*** TEST SUCCESS DATA RECOVERED INTACT ***" ) ;
  else
    puts( "\n*** TEST FAILED ***" ) ;
  return outcome ;
}

void testunbase64withbadascii()
{
  int i ;
  // BAD base64 data.  These numbers represent non-base64 alphabet characters.
  // if BASE64PARANOIA is on, then unbase64() will catch it and send back a NULL
  // pointer.  Otherwise, you will just get invalid data back (but the program should not
  // crash).
  char badAscii[] = { -1, -3, -128, 127, 20, 10, 36, 92, 50, 126, 0, 0, 5, 0 } ;
  int badAsciiLen = sizeof( badAscii ) ;
  unsigned char *baddat ;
  int baddatLen ;
  
  puts( ">> NOW TESTING UNBASE64 WITH INVALID DATA:" ) ;
  for( i = 0 ; i < badAsciiLen; i++ )
    printf( "%d, ", badAscii[i] ) ;
  printf("\n<< EXPECTED >> ");
    
  // check the integrity ( it will show it's not valid base64 )
  if( !base64integrity( badAscii, badAsciiLen ) )
  {
    puts( "There are some invalid ascii characters in your base64 string" ) ;
    baddat = unbase64( badAscii, badAsciiLen, &baddatLen ) ;
    
    puts( "The unbase64'd data, anyway, is:" ) ;
    for( i = 0 ; i < baddatLen ; i++ )
      printf( "%d, ", baddat[i] ) ;
    puts("");
    free( baddat ) ;
  }
}



// -- other util --
// Prints the unb64 array in base64.h
void printUnbase64()
{
  int i;
  puts( "const static unsigned char unb64[]={" ) ;
  for( i = 0 ; i < '0' ; i++ )
  {
    if( i=='+' ) // + is 43
      printf( " 62, " ) ;
    else if( i=='/' ) // / is 47
      printf( " 63, " ) ;
    else
      printf( "  0, " ) ;
    if( i && (i+1)%10==0 ) printf("//%d \n", (i+1) );
  }
  for( i = '0' ; i <= '9' ; i++ )
  {
    printf( "%3d, ", i-'0' + 52 ) ; // '0'=>52, like a deck of cards. Go Nana.
    if( i && (i+1)%10==0 ) printf("//%d \n", (i+1) );
  }
  for( i = '9'+1 ; i < 'A' ; i++ )
  {
    printf( "  0, " ) ;
    if( i && (i+1)%10==0 ) printf("//%d \n", (i+1) );
  }
  for( i = 'A' ; i <= 'Z' ; i++ )
  {
    printf( "%3d, ", i-'A' ) ;
    if( i && (i+1)%10==0 ) printf("//%d \n", (i+1) );
  }
  for( i = 'Z'+1 ; i < 'a' ; i++ )
  {
    printf( "  0, " ) ;
    if( i && (i+1)%10==0 ) printf("//%d \n", (i+1) );
  }
  for( i = 'a' ; i <= 'z' ; i++ )
  {
    printf( "%3d, ", i-'a'+26 ) ; //'a' has the value of 26
    if( i && (i+1)%10==0 ) printf("//%d \n", (i+1) );
  }
  
  // Now put 0's until 255, in case string sent to be unbase64'd
  // contains illegal characters
  for( ; i < 256 ; i++ )
  {
    printf( "  0, " ) ;
    if( i && (i+1)%10==0 ) printf("//%d \n", (i+1) );
  }
  printf( "\n}; // This array has %d elements\n", i ) ;
  
}

#endif
