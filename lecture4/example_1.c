// Do some I/O and math with unsigned types.

#include <stdlib.h>
#include <stdio.h>



int main()
{

 int x = 10;
 {
   x++;
   int x = 5;
   {
     printf( "%d\n", x );
   }
 }

  return EXIT_SUCCESS;
}
