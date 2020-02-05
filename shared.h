// shared.h
// Midn Alam
// 150084

#ifndef _SHARED_H
#define _SHARED_H

#include <sys/types.h>
#include <semaphore.h>

#define MATCHES 0
#define PAPER   1
#define TOBACCO 2

// Binary semaphores 
extern sem_t empty, full;

// items placed on table
extern int item[2];

#endif
