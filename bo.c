//Description: IC221 Spring 2013, Meeting 33-38, Byte Order

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[]) {
  //      bit-sequence:
  //   0000 0001   0000 0010
  //
  // +-----------+-----------+
  // | 0000 0001 | 0000 0010 |
  // +-----------+-----------+
  //     most        least
  //  significant significant
  //     byte        byte
  //    (MSB)       (LSB)

  //    ------------------> increasing memory addresses
  //       p         p+1
  // +-----------+-----------+
  // |    MSB    |   LSB     | Big-endian:  MSB stored at lowest memory address
  // +-----------+-----------+
  // |    LSB    |   MSB     | Little-endian: LSB stored at lowest memory address
  // +-----------+-----------+

  // 2 sequential bytes in memory:
  // 
  char *p = (char *)calloc( 2, sizeof(char) );
  *p       = 0x01; // 0000 0001
  *(p + 1) = 0x02; // 0000 0010

  // If this architecture is big-endian (MSB @ lowest address)
  // +-----------+-----------+
  // | 0000 0001 | 0000 0010 | => 258
  // +-----------+-----------+
  //     p = MSB   p+1 = LSB

  // If this architecture is little-endian (LSB @ lowest address)
  // +-----------+-----------+
  // | 0000 0010 | 0000 0001 | => 513
  // +-----------+-----------+
  //   p+1 = MSB     p = LSB

  unsigned short int *q;
  q = (unsigned short *)p;

  printf( "\n" );
  printf( "         bits: 0000 0001  0000 0010\n" );
  printf( "\n" );
  printf( "   big endian:    MSB        LSB\n" );
  printf( "   (sparc)     0000 0001  0000 0010 => 258\n" );
  printf( "\n" );
  printf( "little endian:    LSB        MSB\n" );
  printf( "   (x86)       0000 0001  0000 0010 => 513\n" );
  printf( "\n" );
  printf( "This architecture is: " ); fflush(stdout); system( "arch" );

  printf( "%i\n", *q );

  return 0;
}
