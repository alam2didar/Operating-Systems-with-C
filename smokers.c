// smokers.c
// Midn Alam
// m150084

#include <stdio.h>
#include <stdlib.h>
#include "shared.h"
#include "smokers.h"

void* paper(void* p)
{
  while( 1 )
  {
    sem_wait(&full);
    
    if(item[0]!= PAPER && item[1] != PAPER){
      printf( "PAPER\n" );
      sem_post(&empty);
    }
    else{
      sem_post(&full);
    }
        
    sleep( 1 + rand()%3 );
  }
}

void* matches(void* p)
{
  
  while( 1 )
  {
    sem_wait(&full);
    
    if(item[0]!= MATCHES && item[1] != MATCHES){
      printf( "MATCHES\n" );
      sem_post(&empty);
    }
    else{
      sem_post(&full);
    }
    
    sleep( 1 + rand()%3 );
  }
}

void* tobacco(void* p)
{
  while( 1 )
  {
    sem_wait(&full);
    
    if(item[0]!= TOBACCO && item[1] != TOBACCO){
      printf( "TOBACCO\n" );
      sem_post(&empty);
    }
    else{
      sem_post(&full);
    }
    
    sleep( 1 + rand()%3 );
  }
}
