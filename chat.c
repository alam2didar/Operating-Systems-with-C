// Description: IC221 Spring 2013 Lecture 33-38 Simple TCP Chat Server/Client

// chat.c
// Client and server TAKE TURNS, with client going FIRST.
// Send "quit" to gracefully exit.

#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

void recv_fm( char *line );
void send_to( char *line );
void sigint( int signo );

int intSockComms;

void usage( char *prog )
{
	printf( "Usage as server: %s <port>\n", prog );
	printf( "      as client: %s <port> <server hostname or IPv4 address>\n", prog );
}

int main( int argc, char *argv[] )
{
	if( argc < 2 || argc > 3 ) // server: argc == 2
	{                          // client: argc == 3
		usage( argv[0] );
		exit( 0 );
	}

	char flgServer = (argc == 2);

	struct addrinfo *ai;
	struct addrinfo            ai_hint = {0};
	               ai_hint.ai_family   = AF_INET;
	               ai_hint.ai_socktype = SOCK_STREAM;
	               ai_hint.ai_protocol = IPPROTO_TCP;
	if ( flgServer )  ai_hint.ai_flags = AI_PASSIVE; // Use any server address

  // Build a sock_addr struct to be used for bind() or connect()
	int intError;
	if( flgServer )  intError = getaddrinfo( NULL, argv[1], &ai_hint, &ai );
	else             intError = getaddrinfo( argv[2], NULL, &ai_hint, &ai );

	if( intError )
	{
		fprintf( stderr, "No address info\n" );
		exit( 0 );
	}

  // Fill in the protocol part of the association
	int intSockListen = socket( ai->ai_family, ai->ai_socktype, ai->ai_protocol );
	if( intSockListen == -1 )
	{
		perror( "socket() failed" );
		exit( 1 );
	}
	printf( "(socket)\n" );
 
	if( flgServer )
	{
		// Fill in the local address/local port part of the association
		if( -1 == bind( intSockListen, ai->ai_addr, ai->ai_addrlen ) )
		{
			perror( "Can't bind socket" );
			exit( 2 );
		}
		printf( "(bind)\n" );

		printf( "(listen)\n" );
		listen( intSockListen, 2 );

		struct sockaddr_in ca = {0};
		socklen_t clen = sizeof( ca );

		// Get the foreign address/foreign port part of the association (client's info)
		intSockComms = accept( intSockListen, (struct sockaddr *)&ca, &clen );
		if( -1 == intSockComms )
		{
			perror( "Accept error" );
			exit(3);
		} 
		printf( "(accept)\n" );
	}
  else // client
	{
		// Server's port:
		((struct sockaddr_in *)ai->ai_addr)->sin_port = htons( atoi(argv[1]) );

		// Fill in the local address/local port/foreign address/foreign port part of the association
		if( -1 == connect( intSockListen, ai->ai_addr, ai->ai_addrlen ) )
		{
			perror( "Can't connect to server" );
			exit( 2 );
		}

		printf( "(connect)\n" );
		intSockComms = intSockListen;
	}

	// Catch ^C to terminate gracefully (shutdown both ends of the socket)
	struct sigaction siga;
	siga.sa_handler = sigint;
	sigaction( SIGINT, &siga, NULL );

	char line[256];
	while( 1 )
	{
		if( flgServer ) recv_fm(line);

		fgets( line, 256, stdin );
		send_to(line);

		if( !flgServer ) recv_fm(line);
	}

	return 0;
}

void sigint( int signo )
{
	printf( "\nexiting...\n" );
	sleep( 3 );
	shutdown( intSockComms, SHUT_RDWR );
	close( intSockComms );
	exit( 0 );
}

void send_to( char *line )
{
	line[ strlen(line) - 1 ] = 0;
	send( intSockComms, line, 1 + strlen(line), 0 );
	if( strcmp( line, "quit" ) == 0 )
		sigint( 0 );
}

void recv_fm( char *line )
{
	int n = recv( intSockComms, line, 256, 0 );
	line[n] = 0;
	printf( "%s\n", line );
	if( strcmp( line, "quit" ) == 0 )
		sigint( 0 );
}
