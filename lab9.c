//#include <termios.h>
#include <unistd.h>
int main(){
  struct termios settings;
  tcgetattr(0, &settings);
  return 0;
}
