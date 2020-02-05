// Midn Alam
// 150084
// shared.c

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#include "shared.h"
#include "agent.h"
#include "smokers.h"

// Define the semaphores declared in shared.h /////
  
sem_t empty, full;

int item[2];

int main (int argc, char *argv[])
{
  srand( time(0) );

  // Initialize semaphores: /////////////////////////
  if( ( -1 == sem_init(&empty,0,1) ) ||  // Initialize Empty
      ( -1 == sem_init(&full,0,0) ) ) // Initialize Full
  { 
    perror( "sem_init" );
    exit( 1 );
  }

  // Create threads for paper, matches, tobacco, and agent:
  pthread_t ta, tp, tm, tt;
  pthread_create(&ta,NULL,agent, NULL);
  pthread_create(&tp,NULL,paper,NULL);
  pthread_create(&tm,NULL,matches,NULL);
  pthread_create(&tt,NULL,tobacco,NULL);

  void *t_ret;
  // wait for the agent:
  pthread_join( ta, &t_ret );

  return 0;
}
