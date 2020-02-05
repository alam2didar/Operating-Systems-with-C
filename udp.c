/*
  Course: IC221 - Systems Programming
  Term: AY2013 Spring
  File: udp.c
  Description: Simple UDP Client-Server; client sends a character to the server, server outputs character received from client.
  Note: Ubuntu 10.04 LTS does not come with coding support for UDP sockets, have to add coding support for UDP sockets. In Ubuntu Software Center search for "happycoder"; Install Generic C++ Library ... UDP/TCP Socket (happycoders-libsocket), and C++ utilities ... modern debugging (happycoders-libdbg)
  Compile: gcc udp.c -lnsl -lsocket -o udp
*/

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int socdSocket;

void usage( char *prog ) {
  printf( "Usage as server: %s <port>\n", prog );
  printf( "      as client: %s <port> <server hostname or IPv4 address>\n", prog );
}

void sigint( int signo ) {
  printf( "\nexiting...\n" );
  sleep( 3 );
  shutdown( socdSocket, SHUT_RDWR );
  close( socdSocket );
  exit( 0 );
}

int main( int argc, char *argv[], char *envp[] ) {
  if ( argc < 2 || argc > 3 ) {  // server: argc == 2
    usage( argv[0] );            // client: argc == 3
    exit( 0 );
  }

  int server = (argc == 2);

  // Catch ^C to terminate gracefully (shutdown both ends of the socket)
  struct sigaction siga;
  siga.sa_handler = sigint;
  siga.sa_flags = SA_RESTART;
  sigaction( SIGINT, &siga, NULL );

  // Fill in the protocol part of the association 
  socdSocket = socket( AF_INET, SOCK_DGRAM, 0 );
  if ( socdSocket == -1 ) {
    perror( "socket() failed" );
    exit( 1 );
  }
  printf( "(socket)\n" );

  // client
  if ( !server ) {
    // Client's address/port
    struct sockaddr_in socaddrClient = {0};
    socaddrClient.sin_family      = AF_INET;
    socaddrClient.sin_port        = htons( 0 );          // Use any
    socaddrClient.sin_addr.s_addr = htonl( INADDR_ANY ); // Use any
    socklen_t socaddrlnClient = sizeof( socaddrClient );

    // Fill in the client's address/port part of the association
    if ( -1 == bind( socdSocket, (struct sockaddr *)&socaddrClient, socaddrlnClient ) ) {
      perror( "Can't bind socket" );
      exit( 2 );
    }
    printf( "(bind)\n" );

    struct hostent *hostServer = gethostbyname( argv[2] );
    if( hostServer == NULL )
    { 
      fprintf( stderr, "No host with that name.\n" );
      exit(0);
    }
    printf( "(gethostbyname)\n" );

    // Fill in the server's address/port part of the association
    struct sockaddr_in socaddrServer = {0};
    socaddrServer.sin_family      = AF_INET;
    socaddrServer.sin_port        = htons( atoi(argv[1]) );
    socaddrServer.sin_addr.s_addr = *(unsigned int *)hostServer->h_addr_list[0];

    while( 1 )
    {
      char c = getchar();
      if( c == '\n' ) continue;

      if( -1 == sendto( socdSocket, &c, sizeof(char), 0,
                        (struct sockaddr *)&socaddrServer,
                        (socklen_t) sizeof( socaddrServer ) ) ) { 
        perror( "sendto() error" );
        break;
      }

      if( c == 'q' )  break;
    }

    sigint( 0 );
  } else {  //server
    // Server's address/port info
    struct sockaddr_in socaddrServer = {0};
    socaddrServer.sin_family      = AF_INET;
    socaddrServer.sin_port        = htons( atoi( argv[1] ) ); // Service will be on this port
    socaddrServer.sin_addr.s_addr = htonl( INADDR_ANY );      // Use any of the server's IP addresses

    // Fill in the local address/local port part of the association
    if( -1 == bind( socdSocket, (struct sockaddr *)&socaddrServer,
                    (socklen_t) sizeof( socaddrServer ) ) ) {
      perror( "Can't bind socket" );
      exit( 2 );
    }
    printf( "(bind)\n" );

    while( 1 ) {
      // Client's address/port info
      struct sockaddr_in socaddrClient = {0};
      socklen_t socaddrlnClient = sizeof( socaddrClient );

      char c = 0;
      if( -1 == recvfrom( socdSocket, &c, sizeof(char), 0,
                          (struct sockaddr *)&socaddrClient, &socaddrlnClient ) ) {
        perror( "recvfrom() error" );
        break;
      } else {
        // Get the IP address from the datagram:
        char chrClientIPAddr[INET_ADDRSTRLEN];
        inet_ntop( AF_INET, &(socaddrClient.sin_addr), chrClientIPAddr, INET_ADDRSTRLEN );

        char chrClientHostName[NI_MAXHOST];
        getnameinfo( (struct sockaddr *)&socaddrClient, socaddrlnClient,
                     chrClientHostName, NI_MAXHOST, NULL, 0, NI_NOFQDN );

        printf( "%s (%s) says: ", chrClientHostName, chrClientIPAddr );
        if( c == 'q' ) printf( "BYE!\n"  );
        else           printf( "%c\n", c );
      }
    }
    sigint( 0 );
  }

  return 0;
}
