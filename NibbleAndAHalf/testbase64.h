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
  printf( "\n}; // This array has %d elements\n", i-1 ) ;
  
}

// Function for automated testing of base64.h.  Also times.
int testbase64( const void* data, int dataLen )
{
  // main ptrs
  unsigned char* binaryPtr = (unsigned char*)data ;
  char* base64dAscii ;
  unsigned char* recoveredData ;
  
  int outcome=1;
  int resLen, recoveredLen ;
  int i ; //compare loop counter
  CTimer t; // for timing runs
  
  printf( "Base64 test with " ) ;
  if( dataLen < 1<<10 )  printf( "%d Bytes", dataLen ) ;
  else if( dataLen < 1<<20 )  printf( "%d KB", dataLen >> 10 ) ;
  else if( dataLen < 1<<30 )  printf( "%d MB", dataLen >> 20 ) ;
  else  printf( "%d GB", dataLen >> 30 ) ;
  printf( " of data\n" ) ;
  
  CTimerInit( &t ) ;
  base64dAscii = base64( data, dataLen, &resLen ) ;
  if( !base64dAscii )  return 0 ; //memory failure
  printf( "base64 %f seconds\n", CTimerGetTime( &t ) ) ;
  
  CTimerReset( &t ) ;
  recoveredData = unbase64( base64dAscii, resLen, &recoveredLen ) ;
  if( !recoveredData )  return 0 ; //memory failure
  printf( "unbase64 %f seconds\n", CTimerGetTime( &t ) ) ;
  
  #ifdef BASE64TESTSHOWDATA
  puts( "Original text:" ) ;
  puts( "--------------------" ) ;
  puts( (char*)data ) ;
  puts( "--------------------" ) ;
  puts( "Base64'd ascii text:" ) ;
  puts( "--------------------" ) ;
  puts( base64dAscii ) ;
  puts( "--------------------" ) ;
  #endif
  
  printf( "base64: %d bytes => %d bytes => %d bytes\n", dataLen, resLen, recoveredLen ) ;
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
    #ifdef BASE64TESTSHOWDATA
    printf( "\n%4d  | %3d | %3d |", i, binaryPtr[i], recoveredData[i] ) ;
    #endif
    if( binaryPtr[i] != recoveredData[i] )
    {
      printf( " X" ) ; //\nERROR: byte @ %d != original data: (%d != %d)\n", i, data[i], recoveredData[i] );
      outcome = 0 ;
    } 
  }
  
  free( base64dAscii ) ;
  free( recoveredData ) ;
  if( outcome )
    puts( "\n*** TEST SUCCESS DATA RECOVERED INTACT ***" ) ;
  else
    puts( "\n*** TEST FAILED ***" ) ;
  return outcome ;
}

void testunbase64withbaddat()
{
  int outLen ;
  int i ;
  // BAD base64 data.  These numbers represent non-base64 alphabet characters.
  // if BASE64PARANOIA is on, then unbase64() will catch it and send back a NULL
  // pointer.  Otherwise, you will just get invalid data back (but the program should not
  // crash).
  char baddat[] = { -1, -3, -128, 127, 20, 10, 36, 92, 50, 126, 0, 0, 5, 0 } ;
  int baddatLen = sizeof( baddat ) ;
  unsigned char *recoveredBaddat ;
  
  puts( "TESTING UNBASE64 WITH INVALID DATA:" ) ;
  for( i = 0 ; i < baddatLen; i++ )
    printf( "%d, ", baddat[i] ) ;
  printf("\n<< EXPECTED >> ");
  
  recoveredBaddat = unbase64( baddat, baddatLen, &outLen ) ;
  if( !recoveredBaddat )
  {
    puts( "BASE64PARANOIA is on, so you get NULL ptr back" ) ;
  }
  else
  {
    printf( "BASE64PARANOIA is OFF, so unbase64'd it anyway.  Got %d bytes:\n", outLen ) ;
    for( i = 0 ; i < outLen ; i++ )
      printf( "%d, ", recoveredBaddat[i] ) ;
    puts("");
    free( recoveredBaddat ) ;
  }
}


#endif
