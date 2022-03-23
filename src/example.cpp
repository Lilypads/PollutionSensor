/*#include "sqr.h"
#include "sqr.cpp"
#include "iostream"
#include "sps30.h"
//#include "sps30lib.h"

int main()
{
std::cout << "Hello World!" << sqr(2) << std::endl;
return 0;
}
    */


#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "try1.h"

int main()
{

SPS30 mySPS;

fprintf(stderr,"Press any key to stop.\n");

SPS30settings s;
mySPS.start(s);
mySPS.readVersion(s);

getchar();

mySPS.stop(s);
 return 0;
}