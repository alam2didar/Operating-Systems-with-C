//Description: IC221 Spring 2013, Meeting 33-38
// resolve.c

#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main( int argc, char *argv[] )
{
  char str[NI_MAXHOST]; // C-string to hold a name or dotted quad

  if( argc == 2 ) strcpy( str, argv[1] );         // look up the named host,
  else            gethostname( str, NI_MAXHOST ); // or just look up this host

  struct addrinfo *ai;
  struct addrinfo  ai_hint = {0};
                   ai_hint.ai_flags  = AI_CANONNAME;
                   ai_hint.ai_family = AF_INET;

  int err = getaddrinfo( str, NULL, &ai_hint, &ai );
  if( 0 != err )
  {
    fprintf( stderr, "No address info for %s:\n", str, gai_strerror(err) );
    exit( 0 );
  }
  else
  {
    // Convert address info into hostname
    char host[NI_MAXHOST];
    getnameinfo(ai->ai_addr, ai->ai_addrlen, host, NI_MAXHOST, NULL, 0, 0);

    // We need the IPv4 32-bit integer address
    struct sockaddr_in *p_sa = (struct sockaddr_in *)ai->ai_addr;
           struct in_addr ia = p_sa->sin_addr;
              in_addr_t addr = ia.s_addr;

    // Convert network form (32-bit address) into dotted quad string
    char dq[INET_ADDRSTRLEN];
    inet_ntop( ai->ai_family, &addr, dq, INET_ADDRSTRLEN );

    printf( "%s => %s at %s ", str, host, dq );
    if( ai->ai_canonname ) printf( "(%s)", ai->ai_canonname );
    printf( "\n" );
  }

  freeaddrinfo(ai); // release the dynamically alloc'd memory

  return 0;
}
