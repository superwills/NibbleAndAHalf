//
//  testbase64.c
//  NibbleAndAHalf
//
//  Created by William Sherif on 2022-02-01.
//  Copyright © 2022 William Sherif. All rights reserved.
//

#include "testbase64.h"
#include "base64.h"

int BASE64TESTSHOWDATA = 0;
const int BASE64TESTMAXDATALEN=1<<27; // Tests up to 128 MB

// Function for automated testing of base64.h.  Also times.
int testbase64( const void* data, int dataLen )
{
  unsigned char* binaryPtr = (unsigned char*)data ;
  
  printf( "Base64 test with " ) ;
  if( dataLen < 1<<10 )  printf( "%d Bytes ", dataLen ) ;
  else if( dataLen < 1<<20 )  printf( "%d KB ", dataLen >> 10 ) ;
  else if( dataLen < 1<<30 )  printf( "%d MB ", dataLen >> 20 ) ;
  else  printf( "%d GB ", dataLen >> 30 ) ;
  puts( "of data" ) ;
  
  CTimer t;
  CTimerInit( &t ) ;
  int base64AsciiLen;
  char* base64Ascii = base64( data, dataLen, &base64AsciiLen ) ;
  if( !base64Ascii )  return 0 ; //memory failure
  printf( "base64 %f seconds\n", CTimerGetTime( &t ) ) ;
  
  CTimerReset( &t ) ;
  if( base64integrity( base64Ascii, base64AsciiLen ) ) // Check the integrity of the base64'd string
    puts( "All base64 encoded data are valid base64 alphabet characters" ) ;
  else
    puts( "ERROR: Bad base64 characters detected" ) ;
  printf( "base64 integrity check %f seconds\n", CTimerGetTime( &t ) ) ;
  
  CTimerReset( &t );
  int recoveredLen;
  char* recoveredData = (char*)unbase64( base64Ascii, base64AsciiLen, &recoveredLen ) ;
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
  int outcome = 1;
  if( dataLen != recoveredLen )
  {
    puts( "ERROR: length( unbase64( base64( data ) ) ) != length( data )" ) ;
    puts( "TEST FAILED" ) ;
    outcome = 0 ;
  }
  // Data is the exact same len. Good.
  else for( int i = 0 ; i < dataLen ; i++ ) // good ol' else for
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
  // BAD base64 data.  These numbers represent non-base64 alphabet characters.
  // if BASE64PARANOIA is on, then unbase64() will catch it and send back a NULL
  // pointer.  Otherwise, you will just get invalid data back (but the program should not
  // crash).
  char badAscii[] = { -1, -3, -128, 127, 20, 10, 36, 92, 50, 126, 0, 0, 5, 0 } ;
  int badAsciiLen = sizeof( badAscii ) ;
  
  puts( ">> NOW TESTING UNBASE64 WITH INVALID DATA:" ) ;
  for( int i = 0 ; i < badAsciiLen; i++ )
    printf( "%d, ", badAscii[i] ) ;
  printf("\n<< EXPECTED >> ");
    
  // check the integrity ( it will show it's not valid base64 )
  if( !base64integrity( badAscii, badAsciiLen ) )
  {
    puts( "There are some invalid ascii characters in your base64 string" ) ;
    int baddatLen ;
    unsigned char *baddat = unbase64( badAscii, badAsciiLen, &baddatLen ) ;
    
    puts( "The unbase64'd data, anyway, is:" ) ;
    for( int i = 0 ; i < baddatLen ; i++ )
      printf( "%d, ", baddat[i] ) ;
    puts("");
    free( baddat ) ;
  }
}

// -- other util --
// Prints the unb64 array in base64.h
void printUnbase64()
{
  int i = 0;
  puts( "const static unsigned char unb64[] = {" ) ;
  for( ; i < '0' ; i++ )
  {
    if( i=='+' ) // + is 43
      printf( " 62, " ) ;
    else if( i=='/' ) // / is 47
      printf( " 63, " ) ;
    else
      printf( "  0, " ) ;
    if( i && (i+1)%10==0 ) printf("//%d \n", (i+1) );
  }
  for( ; i <= '9' ; i++ )
  {
    printf( "%3d, ", i-'0' + 52 ) ; // '0'=>52, like a deck of cards. Go Nana.
    if( i && (i+1)%10==0 ) printf("//%d \n", (i+1) );
  }
  for( ; i < 'A' ; i++ )
  {
    printf( "  0, " ) ;
    if( i && (i+1)%10==0 ) printf("//%d \n", (i+1) );
  }
  for( ; i <= 'Z' ; i++ )
  {
    printf( "%3d, ", i-'A' ) ;
    if( i && (i+1)%10==0 ) printf("//%d \n", (i+1) );
  }
  for( ; i < 'a' ; i++ )
  {
    printf( "  0, " ) ;
    if( i && (i+1)%10==0 ) printf("//%d \n", (i+1) );
  }
  for( ; i <= 'z' ; i++ )
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

void failingTest()
{
  const char Base64[]="a/==";
  uint8_t *raw_data;
  int binary_length;
 
  raw_data=unbase64(Base64,sizeof(Base64),&binary_length);
  printf("BIN LEN=%d\n", binary_length);
}

void testUnbase64InvalidInput()
{
  // As Christian Reitter pointed out, there are some input sequences
  // that used to cause unbase64 to err-out.
}

void automatedTests()
{
  int allOk=1;
  
  //srand( 220 ); // want same sequences
  srand( (unsigned int)time(0) ) ;
  
  for( int testDatLen = 1 ; testDatLen <= BASE64TESTMAXDATALEN ; testDatLen <<= 1 )
  {
    unsigned char *dat = (unsigned char*)malloc( testDatLen ) ;
    for( int i = 0 ; i < testDatLen ; i++ )
      dat[i]=rand(); // make new random data
      
    allOk &= testbase64( dat, testDatLen ) ;
    
    free( dat ) ;
    
    if( !allOk )  break ;
  }
  
  if( allOk )  puts( "-- ALL TESTS COMPLETED SUCCESSFULLY --" ) ;
  else puts( "ERROR: At least one test failed. You should check it out." ) ;
}
