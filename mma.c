#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <utime.h>
#include <pwd.h>



int main (int argc, double argv[]){

  double max = 0; 
  double min = 0; 
  int i[argc], j,c;
  double avg = 0;
  double sum = 0;
                  
  /* for(j=0; j<argc; j++){ */

  /*   scanf("%i",&(i[j])); */
  /* } */

  for (c = 1; c <argc ; c++)
  {
                  
    if (argv[c] > max) {
      max = i[c];
    }

    if (min == 0) {
      min = argv[c];
    } 
    else if(argv[c] < min) {
      min = argv[c];
    }

    sum = sum + argv[c];
  }
  avg=sum/argc;
  printf("sum = %f\n",sum);
  printf("maximum value = %f\n", max);
  printf("minimum value =%f \n", min);
  printf("avg = %f \n",avg);
 
  
  return 0; 
}



