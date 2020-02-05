/*
  Course: IC221 - Systems Programming
  Term: AY2013 Spring
  File: get.c
  Description: Activity: Start of a web client, send a GET request, and receive the reply
*/

#include <libgen.h>       // basename()
#include <netdb.h>        // Networking
#include <stdio.h>        // Need I/O
#include <stdlib.h>       // NULL
#include <unistd.h>       // close()
#include <sys/socket.h>   // Sockets
#include <netinet/in.h>   // Networking

#define READ_END 0
#define WRITE_END 1

int main( int argc, char *argv[], char *envp[] ) {

	if( argc == 1 ) {
		printf( "Usage: %s <hostname | dotted quad>\n", basename(argv[0]) );
		exit( 0 );
	}

	struct addrinfo *socAddrInfo;  // Pointer to struct addrinfo to act as head for linked list returned by getaddrinfo()
	struct addrinfo  socAddrInfoHint = {0};
	     socAddrInfoHint.ai_family   = AF_INET;      // IPv4 addresses only
	     socAddrInfoHint.ai_socktype = SOCK_STREAM;  // stream socket
	     socAddrInfoHint.ai_protocol = IPPROTO_TCP;  // TCP

	if( 0 != getaddrinfo( argv[1], NULL, &socAddrInfoHint, &socAddrInfo ) ) {
		fprintf( stderr, "No address info for %s\n", argv[1] );
		exit( 0 );
	}

	int socdHTTPClient = socket( socAddrInfo->ai_family, socAddrInfo->ai_socktype, socAddrInfo->ai_protocol );
	if( -1 == socdHTTPClient ) {
		freeaddrinfo( socAddrInfo );  // We failed to get a socket, but need to clean up
		perror( "socket() error.\n" );
		exit( 1 );
	}

	// Fill in the foreign port part of the association:
	((struct sockaddr_in *)socAddrInfo->ai_addr)->sin_port = htons( 80 );

	// Try to connect to the server:
	if( -1 == connect( socdHTTPClient, socAddrInfo->ai_addr, socAddrInfo->ai_addrlen ) ) {
		// We failed to connect, but need to clean up
		// Clean up socket
		shutdown( socdHTTPClient, SHUT_RDWR );
		close( socdHTTPClient );
		// Clean up search results
		freeaddrinfo( socAddrInfo );
		perror( "connect() error.\n" );
		exit( 2 );
	} else {
		freeaddrinfo( socAddrInfo );  // We are connected and no longer need our search results
		
		#ifndef SIMILAR_TO_PIPE  // We are not thinking like a pipe(), use separate FILE * for reading and writing
		
		// Unlike a pipe, sockets are full-duplex (we can read-from/write-to a socket)
		// And HTTP communications are text strings
		FILE *socsHTTPFromServer = fdopen( socdHTTPClient, "r" );
		FILE *socsHTTPToServer  = fdopen( socdHTTPClient, "w" );
		
		setbuf( socsHTTPToServer, NULL );

		// Speak some HTTP to the server:
		fprintf( socsHTTPToServer, "GET / HTTP/1.0\n\n" );    

		char chrEcho;
		while( 1 == fscanf( socsHTTPFromServer, "%c", &chrEcho ) )
			printf( "%c", chrEcho ); 
		printf("\n");
		
		//Clean up FILE *
		fclose( socsHTTPFromServer );
		fclose( socsHTTPToServer );
		
		#else  // We are thinking like a pipe(), use an array of two FILE *
		
		FILE *socfHTTPFromToServer[2];
		
		socfHTTPFromToServer[READ_/* ???*/] = fdopen( /* ??? */, "r" );
		socfHTTPFromToServer[WRITE_/* ???*/] = fdopen( /* ??? */, "w" );
		
		setbuf( /* ??? */, NULL );

		// Speak some HTTP to the server:
		fprintf( /* ??? */, "GET / HTTP/1.0\n\n" );    

		char chrEcho;
		while( 1 == fscanf( /* ??? */, "%c", &chrEcho ) )
			printf( "%c", chrEcho ); 
		printf("\n");
		
		// Clean up FILE *
		fclose( socfHTTPFromToServer[READ_/* ??? */] );
		fclose( socfHTTPFromToServer[WRITE_/* ??? */] );
		
		#endif
	}
	
	// Clean up socket
	shutdown( socdHTTPClient, SHUT_RDWR );
	close( socdHTTPClient );

	return 0;
}
