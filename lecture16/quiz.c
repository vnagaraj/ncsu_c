#include <stdio.h>
#include <stdlib.h>

unsigned short increment4to7( unsigned short s )
{
  unsigned short bitField = ( s & 0x00F0 ) >> 4;
  bitField++;
  s &= 0xFF0F;
  s |=  ( ( bitField << 4 ) & 0x00F0 );
  return s;
}

int main()
{
  unsigned short result = increment4to7(0x5678);
  printf( "%04x\n", result);
  return EXIT_SUCCESS;
}
