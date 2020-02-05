// agent.c
// Midn Alam
// 150084

#include <stdio.h>
#include <stdlib.h>
#include "shared.h"
#include "agent.h"
#include "smokers.h"

void* agent(void *p)
{
  while (1)
  {
    // randomly choose 2 items
    double n = (double)rand();
    int i = 1;
    if( n < 0.333333*RAND_MAX ) i = 0;
    if( n > 0.666666*RAND_MAX ) i = 2;


    sem_wait(&empty);
    
    // place them on the table
    switch( i )
    {
    case PAPER:   item[0] = MATCHES;
      item[1] = TOBACCO;
      printf( "AGENT: MATCHES and TOBACCO => " );
      break;

    case MATCHES: item[0] = PAPER;
      item[1] = TOBACCO;
      printf( "AGENT: PAPER and TOBACCO => " );
      break;

    case TOBACCO: item[0] = MATCHES;
      item[1] = PAPER;
      printf( "AGENT: MATCHES and PAPER => " );
      break;
    }

    sem_post(&full);

  }
}
