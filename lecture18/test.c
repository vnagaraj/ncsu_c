#include <stdio.h>
#include <stdlib.h>



int main()
{

  char a[ 8 ] = "one";
  char b[ 6 ] = "two";
  char c[ 10 ] = "three";

   strcat( c, b );
  //a[7] = '\0';

  printf("%zd\n", sizeof(c));
  printf("%zd\n", strlen(c));
  printf("%s\n", c);
  return EXIT_SUCCESS;
}
