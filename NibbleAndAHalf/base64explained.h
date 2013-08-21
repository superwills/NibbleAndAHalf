/*
  
  https://github.com/superwills/NibbleAndAHalf
  base64explained.h -- Fast base64 encoding and decoding.
  version 1.0.0, April 17, 2013 143a
  
  EXPLAINS how the functions in base64.h work. You don't need this file,
  only base64.h is needed.

  Copyright (C) 2013 William Sherif

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  William Sherif
  will.sherif@gmail.com

  YWxsIHlvdXIgYmFzZSBhcmUgYmVsb25nIHRvIHVz

*/
#ifndef BASE64_H
#define BASE64_H

// The COMPILE-TIME SETTING BASE64PARANOIA is really important.
// You need to decide if PARANOIA is more important to you than speed.
//
// BASE64PARANOIA: Remove this def to NOT check the validity of base64 ascii strings
// before unbase64'ing that string.  If you don't #define BASE64PARANOIA,
// then the program assumes that all characters in the string sent to unbase64() 
// are in the base64 alphabet.  As such if a character is NOT in the base64 alphabet
// your data will be wrong (it will be turned to 0 (as if it were just a base64 'A')).
// Removing this test greatly speeds up unbase64'ing (by about 3-4x).
#define BASE64PARANOIA

// BASE64 and UNBASE64 MAPS
//  maps 0=>A, 1=>B..63=>/ etc
#if 0
                        ----------1---------2---------3---------4---------5---------6---
                        0123456789012345678901234567890123456789012345678901234567890123
#endif
const static char* b64="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" ;

// maps A=>0,B=>1..
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
}; // This array has 255 elements

// Converts binary data of length=len to base64 characters.
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
  int pad = ((modulusLen&1)<<1) + ((modulusLen&2)>>1) ; // 2 gives 1 and 1 gives 2, but 0 gives 0.
  
  // Could also do 3-modulusLen, but that gives 3 when modulusLen=0.
  // 0 => 0
  // 1 => 1  pad 2
  // 2 => 2  pad 1
  // 3 => 0  no pad
  // 4 => 1  pad 2
  // 5 => 2  pad 1
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

// Checks the integrity of a base64 string to make sure it is
// made up of only characters in the base64 alphabet (array b64)
#ifdef __cplusplus
// Prefer an inline function in C++
inline int isbase64ValidChr( char ch )
{
  return ('0' <= ch && ch <= '9') || // between 0-9
         ('A' <= ch && ch <= 'Z') || // between A-Z
         ('a' <= ch && ch <= 'z') || // between a-z
           ch=='+'   ||   ch=='/' ; // other 2 valid chars, + ending chrs
}
#else
// OK ANSI C, a #define it is
#define isbase64ValidChr( ch ) ( ('0' <= ch && ch <= '9') || \
('A' <= ch && ch <= 'Z') || ('a' <= ch && ch <= 'z') || \
ch=='+' || ch=='/' )
#endif

// Well after writing it and testing the macro above, I noticed
// the isbase64ValidChr macro doesn't perform better than the isbase64ValidChr function,
// even WITHOUT keyword inline.

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

unsigned char* unbase64( const char* ascii, int len, int *flen )
{
  const unsigned char *safeAsciiPtr = (const unsigned char*)ascii ; // internally I use
  // an unsigned char pointer, so that __the maximum value read out is 255,
  // and the value is never negative__.  This is a type of
  // "if statement" enforced by the type of the pointer.
  unsigned char* bin ;
  int cb=0; // counter for bin
  int charNo; // counter for what base64 char we're currently decoding
  int pad = 0 ;
  
#ifdef BASE64PARANOIA
  if( !base64integrity( ascii, len ) )  return 0 ; // NULL PTR if bad integrity.
#endif
  
  // Count == on the end to determine how much it was padded.
  if( safeAsciiPtr[ len-1 ]=='=' )  ++pad ;
  if( safeAsciiPtr[ len-2 ]=='=' )  ++pad ;
  
  // You take the ascii string len and divide it by 4
  // to get #24lets (groups of 3 octets). You then *3 to
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

#endif
