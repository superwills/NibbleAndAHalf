//
//  base64.c
//  NibbleAndAHalf
//
//  Created by William Sherif on 2022-02-01.
//  Copyright © 2022 William Sherif. All rights reserved.
//

#include "base64.h"

// MAPS
// b64 maps 0=>A, 1=>B..63=>/ etc
//                      ----------1---------2---------3---------4---------5---------6---
//                      0123456789012345678901234567890123456789012345678901234567890123
const static char* b64="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" ;

// unb64 maps A=>0, B=>1.. and all others not present in base64 alphabet to 0.
const static unsigned char unb64[]={
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, //10 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, //20 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, //30 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, //40 
  0,   0,   0,  62,   0,   0,   0,  63,  52,  53, //50 
 54,  55,  56,  57,  58,  59,  60,  61,   0,   0, //60 
  0,   0,   0,   0,   0,   0,   1,   2,   3,   4, //70 
  5,   6,   7,   8,   9,  10,  11,  12,  13,  14, //80 
 15,  16,  17,  18,  19,  20,  21,  22,  23,  24, //90 
 25,   0,   0,   0,   0,   0,   0,  26,  27,  28, //100 
 29,  30,  31,  32,  33,  34,  35,  36,  37,  38, //110 
 39,  40,  41,  42,  43,  44,  45,  46,  47,  48, //120 
 49,  50,  51,   0,   0,   0,   0,   0,   0,   0, //130 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, //140 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, //150 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, //160 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, //170 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, //180 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, //190 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, //200 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, //210 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, //220 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, //230 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, //240 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, //250 
  0,   0,   0,   0,   0,   0, 
}; // This array has 256 elements

// Converts any binary data to base64 characters.
// Length of the resultant string is stored in flen
// (you must pass pointer flen).
char* base64( const void* binaryData, int len, int *flen )
{
  // I look at your data like the stream of unsigned bytes that it is
  // Main pointers
  const unsigned char* bin = (const unsigned char*) binaryData ;
  char* res ;
  
  // Counters
  int rc = 0 ; // result counter
  int byteNo ; // I need this after the loop
  
  int modulusLen = len % 3 ;
  
  // PAD. Base64 is all about breaking the input into SEXTETS, or 6-bit inputs.
  // If you have 1 byte of input, that's 8 bits, not 6. So it won't fit.
  // We need the input to be a multiple of 6. So 8 bits would be padded
  // by 2 bytes to make the total input size 24 bits, which is divisible by 6.
  // A 2 byte input is 16 bits, which is not divisible by 6. So we pad it
  // by 1 byte to make it 24 bits, which is now divisible by 6.
  
  // We use modulus 3 bytes above because that's 24 bits, and 24 bits is
  // the lowest number that is both divisible by 6 and 8. We need the final
  // output data is to both be divisible by 6 and 8.
  int pad = ((modulusLen&1)<<1) + ((modulusLen&2)>>1) ; // 2 gives 1 and 1 gives 2, but 0 gives 0.
  
  *flen = 4*(len + pad)/3 ; // (len+pad) IS divisible by 3
  //printf( "dataLen=%d, pad=%d, flen=%d\n", len, pad, flen ) ;
  res = (char*) malloc( *flen + 1 ) ; // and one for the null
  if( !res )
  {
    puts( "ERROR: base64 could not allocate enough memory." ) ;
    puts( "I must stop because I could not get enough" ) ;
    return 0;
  }
  
  for( byteNo = 0 ; byteNo <= len-3 ; // If there WAS padding, skip the last 3 octets and process below.
    // 0=>no, 1=>no, 2=>no, 3=>ONCE,4=>ONCE,5=>ONCE, 6=>2x..
    byteNo+=3 )
  {
    unsigned char BYTE0=bin[byteNo];
    unsigned char BYTE1=bin[byteNo+1];
    unsigned char BYTE2=bin[byteNo+2];
    //printf( "BYTE0=%d BYTE1=%d BYTE2=%d\n", BYTE0, BYTE1, BYTE2 ) ;
    //    bin[0]       bin[1]      bin[2]
    // +-----------+-----------+-----------+
    // | 0000 0011   0111 1011   1010 1101 |
    // +-AAAA AABB   BBBB CCCC   CCDD DDDD
    // A: 0 => A
    // B:39 => n
    // C:46 => u
    // D:45 => t
    ///unsigned char mask11111100 = 0x3f, mask00111111 = 0x3f, mask00111111 = 0x3f ;
    
    // Take first sextet (an octet (byte) is 8 bits, so a sextet is 6 bits, or a nibble and a half.)
    // and find out what number they are.
    res[rc++]  = b64[ BYTE0 >> 2 ] ; // unsigned so 0's always come in from left (even though there is 
    // implicit int promotion on R&L sides prior to actual bitshift).
    // convert that number into the base64 alphabet.
    // the value in 6 bits can never be larger than 63.
    
    // the second sextet is part of the first byte and partly in the 2nd byte.
    res[rc++]  = b64[ ((0x3&BYTE0)<<4) + (BYTE1 >> 4) ] ;  // 1st term: 3<<4 has a maximum value of 110000=48.
    
    // notice how I avoided the scary endian ghost by using a unsigned char* ptr for all this.
    
    // 3rd sextet is lower nibble of 2nd byte and upper half nibble of 3rd byte.
    res[rc++]  = b64[ ((0x0f&BYTE1)<<2) + (BYTE2>>6) ] ; // 1st term: those are the higher order bits
    
    // 4th sextet
    res[rc++]  = b64[ 0x3f&BYTE2 ] ;  // already low order, just mask off 2 hiorder bits
  }
  
  // The last 3 octets must be converted carefully as if len%3==1 or len%3==2 we must
  // "pretend" there are additional bits at the end.
  if( pad==2 ) //len%3==1 (1,4,7,10)
  {
    // We are missing 2 bytes. So
    //   - we will only extract 2 sextets when len%3==1
    //   - The 2nd sextet's 2 HI ORDER BITS, NOT LO-ORDER.
    //   - are being specified by the lowest 2 bits of the 1st octet. these should be 0.
    //    bin[0]       bin[1]      bin[2]
    // +-----------+-----------+-----------+
    // | 0000 0011   ~~~~ ~~~~   ~~~~ ~~~~ |
    // +-AAAA AABB   XXXX XXXX   XXXX XXXX
    res[rc++] = b64[ bin[byteNo] >> 2 ] ;
    res[rc++] = b64[ (0x3&bin[byteNo])<<4 ] ; // "padded" by 0's, these 2 bits are still HI ORDER BITS.
    // Last 2 are ==, to indicate there's been a 2 byte-pad
    res[rc++] = '=';
    res[rc++] = '=';
  }
  else if( pad==1 )
  {
    // When len%3==2 (2,5,8,11) (missing 1 byte).
    //   - 3 sextets (C is 0 padded)
    //    bin[0]       bin[1]      bin[2]
    // +-----------+-----------+-----------+
    // | 0000 0011   1111 1111   ~~~~ ~~~~ |
    // +-AAAA AABB   BBBB CCCC   XXXX XXXX  
    res[rc++]  = b64[ bin[byteNo] >> 2 ] ;
    res[rc++]  = b64[ ((0x3&bin[byteNo])<<4)   +   (bin[byteNo+1] >> 4) ] ; //SEX2 formula
    res[rc++]  = b64[ (0x0f&bin[byteNo+1])<<2 ] ; // only part of SEX3 that comes from byte#1
     
    // Last one is =.
    res[rc++] = '=';
  }
  
  res[rc]=0; // NULL TERMINATOR! ;)
  return res ;
}

int base64integrity( const char *ascii, int len )
{
  // LOOKING FOR BAD CHARACTERS
  int i ;
  for( i = 0 ; i < len - 2 ; i++ )
  {
    if( !isbase64ValidChr( ascii[i] ) ) 
    {
      printf( "ERROR in base64integrity at chr %d. String is NOT valid base64.\n", i ) ;
      return 0 ;
    }
  }
  
  // Only last 2 can be '='
  // Check 2nd last:
  if( ascii[i]=='=' )
  {
    // If the 2nd last is = the last MUST be = too
    if( ascii[i+1] != '=' )
    {
      printf( "ERROR in base64integrity at chr %d.\n"
      "If the 2nd last chr is '=' then the last chr must be '=' too.\n "
      "String is NOT valid base64.", i ) ;
      return 0 ;
    }
  }
  else if( !isbase64ValidChr( ascii[i] ) )  // not = or valid base64
  {
    // 2nd last was invalid and not '='
    printf( "ERROR in base64integrity at chr %d (2nd last chr). String is NOT valid base64.\n", i ) ;
    return 0 ;
  }
  
  // check last
  
  i++ ;
  if( ascii[i]!='=' && !isbase64ValidChr( ascii[i] ) )
  {
    printf( "ERROR in base64integrity at chr %d (last chr). String is NOT valid base64.\n", i ) ;
    return 0 ;    
  }
  
  // Otherwise if get here, b64 string was valid.
  
  return 1 ;
}

// It was recently found there are ways to trip up unbase64 by passing it
// malformed ascii strings. However, you can still trip up the program by
// passing it an incorrect data length.
unsigned char* unbase64( const char* ascii, int len, int *flen )
{
  const unsigned char *safeAsciiPtr = (const unsigned char*)ascii ; // internally I use
  // an unsigned char pointer, so that __the maximum value read out is 255,
  // and the value is never negative__.  This is a type of
  // "if statement" enforced by the type of the pointer.
  // This eliminates a possible bounds check on your array lookups into unb64[]
  // (*(unsigned char*) having values between 0 and 255 means it will always be
  // inside the bounds of the 256 element array).
  unsigned char *bin ;
  int cb=0; // counter for bin
  int charNo; // counter for what base64 char we're currently decoding
  int pad = 0 ;
  
#ifdef BASE64PARANOIA
  if( !base64integrity( ascii, len ) )  return 0 ; // NULL PTR if bad integrity.
#endif
  
  if( len < 2 ) { // 2 accesses below would be OOB.
    // catch empty string, return NULL as result.
    puts( "ERROR: You passed an invalid base64 string (too short). You get NULL back." ) ;
    *flen=0;
    return 0 ;
  }
  
  // You have to ensure the integrity of the base64 string before
  // going ahead and counting the = as pad.
  
  // Count == on the end to determine how much it was padded.
  if( safeAsciiPtr[ len-1 ]=='=' )  ++pad ;
  if( safeAsciiPtr[ len-2 ]=='=' )  ++pad ;
  
  // You take the ascii string len and divide it by 4
  // to get the number of 3 octet groups. You then *3 to
  // get #octets total.
  *flen = 3*len/4 - pad ;
  bin = (unsigned char*)malloc( *flen ) ; //exact len
  if( !bin )
  {
    puts( "ERROR: unbase64 could not allocate enough memory." ) ;
    puts( "I must stop because I could not get enough" ) ;
    return 0;
  }
  
  // NEVER do the last group of 4 characters if either of the
  // last 2 chars were pad.
  for( charNo=0; charNo <= len - 4 - pad ; charNo+=4 )
  {
    // Get the numbers each character represents
    // Since ascii is ONE BYTE, the worst that can happen is
    // you get a bunch of 0's back (if the base64 string contained
    // characters not in the base64 alphabet).
    // The only way unbase64 will TELL you about this though
    // is if you #define BASE64PARANOIA (particularly because
    // there is a 3-4x performance hit, just for the integrity check.)
    int A=unb64[safeAsciiPtr[charNo]];
    //printf( "[%4d] %c => %d\n", charNo, ascii[charNo], A ) ;
    int B=unb64[safeAsciiPtr[charNo+1]];
    //printf( "[%4d] %c => %d\n", charNo+1, ascii[charNo+1], B ) ;
    int C=unb64[safeAsciiPtr[charNo+2]];
    //printf( "[%4d] %c => %d\n", charNo+2, ascii[charNo+2], C ) ;
    int D=unb64[safeAsciiPtr[charNo+3]];
    //printf( "[%4d] %c => %d\n", charNo+3, ascii[charNo+3], D ) ;
    
    // Just unmap each sextet to THE NUMBER it represents.
    // You then have to pack it in bin,
    // we go in groups of 4 sextets, 
    // and pull out 3 octets per quad of sextets.
    //    bin[0]       bin[1]      bin[2]
    // +-----------+-----------+-----------+
    // | 0000 0011   0111 1011   1010 1101 |
    // +-AAAA AABB   BBBB CCCC   CCDD DDDD
    // or them
    bin[cb++] = (A<<2) | (B>>4) ; // or in last 2 bits of B
    
    // The 2nd byte is the bottom 4 bits of B for the upper nibble,
    // and the top 4 bits of C for the lower nibble.
    bin[cb++] = (B<<4) | (C>>2) ;
    bin[cb++] = (C<<6) | (D) ; // shove C up to top 2 bits, or with D
  }
  
  if( pad==1 )
  {
    // 1 padding character.
    //    bin[0]       bin[1]      bin[2]
    // +-----------+-----------+-----------+
    // | 0000 0011   1111 1111   ~~~~ ~~~~ |
    // +-AAAA AABB   BBBB CCCC   XXXX XXXX  
    // We can pull 2 bytes out, not 3.
    // We have __3__ characters A,B and C, not 4.
    int A=unb64[safeAsciiPtr[charNo]];
    int B=unb64[safeAsciiPtr[charNo+1]];
    int C=unb64[safeAsciiPtr[charNo+2]];
    
    bin[cb++] = (A<<2) | (B>>4) ;
    bin[cb++] = (B<<4) | (C>>2) ;
  }
  else if( pad==2 )
  {
    //    bin[0]       bin[1]      bin[2]
    // +-----------+-----------+-----------+
    // | 0000 0011   ~~~~ ~~~~   ~~~~ ~~~~ |
    // +-AAAA AABB   XXXX XXXX   XXXX XXXX
    int A=unb64[safeAsciiPtr[charNo]];
    int B=unb64[safeAsciiPtr[charNo+1]];
    
    bin[cb++] = (A<<2) | (B>>4) ;
  }
  
  return bin ;
}
