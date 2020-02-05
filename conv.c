//Description: IC221 Spring 2013, Meeting 33-38
// IPv4 address resolveersion: network form <--> presentation form

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main( int argc, char *argv[] )
{
  unsigned long ul_addr;          // form used by IPv4 protocol
  char str_addr[INET_ADDRSTRLEN]; // dotted quad string form

  printf( "(1) Network form to presentation form\n" );
  printf( "Enter a non-negative integer: " );

  scanf( "%lu", &ul_addr );
  ul_addr = htonl( ul_addr ); // inet_ntop requires network byte order

  // Example: 16909060 => 1.2.3.4
  inet_ntop( AF_INET, &ul_addr, str_addr, INET_ADDRSTRLEN );
  printf( "%s\n", str_addr );

  printf( "(2) Presentation form to network form\n" );
  printf( "Enter an IPv4 address as a dotted quad: " ); 

  scanf( "%s", str_addr );

  // Example: 1.2.3.4 => 16909060
  int success = inet_pton( AF_INET, str_addr, &ul_addr );

  if( success )
  {
    ul_addr = ntohl( ul_addr ); // output in host byte order
    printf( "%lu\n", ul_addr );
  }
  else
    fprintf( stderr, "Not a dotted quad\n" );

  return 0;
}
