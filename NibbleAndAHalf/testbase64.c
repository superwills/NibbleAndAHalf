#include "testbase64.h"
#include "base64.h"

#include <string.h>

int BASE64TESTSHOWDATA = 0;
int SHOWTIMING = 0;
const int BASE64TESTMAXDATALEN=1<<27; // Tests up to 128 MB

// Function for automated testing of base64.h.  Also times.
int testBase64( const void* data, int dataLen )
{
  unsigned char* binaryPtr = (unsigned char*)data ;
  
  printf( "Base64 test with " ) ;
  if( dataLen < 1<<10 )  printf( "%d Bytes ", dataLen ) ;
  else if( dataLen < 1<<20 )  printf( "%d KB ", dataLen >> 10 ) ;
  else if( dataLen < 1<<30 )  printf( "%d MB ", dataLen >> 20 ) ;
  else  printf( "%d GB ", dataLen >> 30 ) ;
  puts( "of data" ) ;
  
  CTimer t;
  CTimerInit( &t );
  int base64StringLen;
  char* base64String = base64( data, dataLen, &base64StringLen );
  if( !base64String )  return 0; //memory failure
  if( SHOWTIMING )  printf( "base64 %f seconds\n", CTimerGetTime( &t ) );
  
  CTimerReset( &t );
  if( !base64integrity( base64String, base64StringLen ) ) // Check the integrity of the base64'd string
  {
    puts( "ERROR: Bad base64 characters detected" );
    free( base64String );
    return 0;
  }
  if( SHOWTIMING )  printf( "base64 integrity check %f seconds\n", CTimerGetTime( &t ) ) ;
  
  CTimerReset( &t );
  int recoveredLen;
  unsigned char* recoveredData = unbase64( base64String, base64StringLen, &recoveredLen );
  if( !recoveredData )
  {
    //memory failure, or invalid base64 data
    puts( "Could not recover data from base64 string" );
    free( base64String );
    return 0;
  }
  if( SHOWTIMING )  printf( "unbase64 %f seconds\n", CTimerGetTime( &t ) ) ;
  
  if( BASE64TESTSHOWDATA )
  {
    puts( "Original data:" ) ;
    for (int i = 0; i < dataLen; i++ )
      putc( binaryPtr[i], stdout );
    puts( "\n--------------------" ) ;
    puts( "Base64'd ascii text:" ) ;
    puts( base64String );
    puts( "--------------------" ) ;
    puts( "Unbase64'd recovered data:" ) ;
    for (int i = 0; i < recoveredLen; i++ )
      putc( recoveredData[i], stdout );
    puts( "\n--------------------" ) ;
  }
  
  printf( "base64: %d bytes => %d bytes => %d bytes\n", dataLen, base64StringLen, recoveredLen ) ;
  puts( "Checking.." ) ;
  int allOk = 1;
  if( dataLen != recoveredLen )
  {
    puts( "ERROR: length( unbase64( base64( data ) ) ) != length( data )" ) ;
    allOk = 0 ;
  }
  // Data is the exact same len. Good.
  else for( int i = 0 ; i < dataLen ; i++ ) // good ol' else for
  {
    if( BASE64TESTSHOWDATA )
      printf( "\n%4d  | %3d | %3d |", i, binaryPtr[i], recoveredData[i] ) ;
    
    if( binaryPtr[i] != recoveredData[i] )
    {
      printf( " X" ) ; //\nERROR: byte @ %d != original data: (%d != %d)\n", i, data[i], recoveredData[i] );
      allOk = 0 ;
    } 
  }
  
  free( base64String ) ;
  free( recoveredData ) ;
  if( allOk )  puts( "\n*** TEST SUCCESS DATA RECOVERED INTACT ***" ) ;
  else  puts( "\n*** TEST FAILED ***" ) ;
  return allOk ;
}

int testBase64String( const char* str )
{
  // Want to see the data output for these small data sizes
  BASE64TESTSHOWDATA = 1;
  puts("Test against a custom string.");
  
  // Notice use of length of strlen(str)+1 to include NULL in the base64 encoding
  return testBase64( str, (int)strlen(str)+1 );
}

void testUnbase64WithBadAscii()
{
  puts("Bad ascii test");
  // BAD base64 data.  These numbers represent non-base64 alphabet characters.
  // if SAFEBASE64 is on, then unbase64() will catch it and send back a NULL
  // pointer.  Otherwise, you will just get invalid data back (but the program should not
  // crash).
  const char* badAscii = "-!#$asdf*()_";
  int badAsciiLen = (int)strlen( badAscii );
  
  puts( ">> NOW TESTING UNBASE64 WITH INVALID BASE64 STRING:" );
  puts( badAscii );
  for( int i = 0 ; i < badAsciiLen ; i++ )
    printf( "%3d, ", badAscii[i] );
  puts("");
  // check the integrity ( it will show it's not valid base64 )
  if( !base64integrity( badAscii, badAsciiLen ) )
  {
    puts( "<< EXPECTED >> There are some invalid ascii characters in your base64 string" );
    int baddatLen;
    unsigned char *baddat = unbase64( badAscii, badAsciiLen, &baddatLen );
    
    printf( "The unbase64'd data is %d bytes, anyway, it is:\n", baddatLen );
    for( int i = 0 ; i < baddatLen ; i++ )
      printf( "%3d, ", baddat[i] );
    puts("");
    free( baddat );
  }
}

void testUnbase64WithBadLength()
{
  puts("Bad length test (not multiple of 4)");
  // As Christian Reitter pointed out, there are some input sequences
  // that used to cause unbase64 to err-out.
  const char* invalidBase64 = "==";
  
  int binaryLen = 0;
  unsigned char* binaryData = unbase64(invalidBase64, (int)strlen(invalidBase64), &binaryLen);
  printf("Binary length=%d\n", binaryLen);
  for( int i = 0; i < binaryLen; i++ )
    printf( "%3d, ", binaryData[i] );
  puts("");
}

void automatedTests()
{
  // Don't show the output for large data
  BASE64TESTSHOWDATA = 0;
  
  puts("Automated tests");
  int allOk=1;
  
  //srand( 220 ); // want same sequences
  srand( (unsigned int)time(0) ) ;
  
  for( int testDatLen = 1 ; testDatLen <= BASE64TESTMAXDATALEN ; testDatLen <<= 1 )
  {
    unsigned char *dat = (unsigned char*)malloc( testDatLen ) ;
    for( int i = 0 ; i < testDatLen ; i++ )
      dat[i]=rand(); // make new random data
      
    allOk &= testBase64( dat, testDatLen ) ;
    
    free( dat ) ;
    
    if( !allOk )  break ;
  }
  
  if( allOk )  puts( "-- ALL TESTS COMPLETED SUCCESSFULLY --" ) ;
  else puts( "ERROR: At least one test failed. You should check it out." ) ;
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
    if( i && isMultipleOf(i+1, 10) ) printf("//%d \n", (i+1) );
  }
  for( ; i <= '9' ; i++ )
  {
    printf( "%3d, ", i-'0' + 52 ) ; // '0'=>52, like a deck of cards. Go Nana.
    if( i && isMultipleOf(i+1, 10) ) printf("//%d \n", (i+1) );
  }
  for( ; i < 'A' ; i++ )
  {
    printf( "  0, " ) ;
    if( i && isMultipleOf(i+1, 10) ) printf("//%d \n", (i+1) );
  }
  for( ; i <= 'Z' ; i++ )
  {
    printf( "%3d, ", i-'A' ) ;
    if( i && isMultipleOf(i+1, 10) ) printf("//%d \n", (i+1) );
  }
  for( ; i < 'a' ; i++ )
  {
    printf( "  0, " ) ;
    if( i && isMultipleOf(i+1,10) ) printf("//%d \n", (i+1) );
  }
  for( ; i <= 'z' ; i++ )
  {
    printf( "%3d, ", i-'a'+26 ) ; //'a' has the value of 26
    if( i && isMultipleOf(i+1, 10) ) printf("//%d \n", (i+1) );
  }
  
  // Now put 0's until 255, in case string sent to be unbase64'd
  // contains illegal characters
  for( ; i < 256 ; i++ )
  {
    printf( "  0, " ) ;
    if( i && isMultipleOf(i+1, 10) ) printf("//%d \n", (i+1) );
  }
  printf( "\n}; // This array has %d elements\n", i ) ;
}
