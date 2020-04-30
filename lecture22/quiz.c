#include <stdio.h>
#include <stdlib.h>

int main()
{
  enum { red, green, blue, purple, orange } x, y;
  x = green;
  y = orange;
  printf( "%d %d\n", x, y );
  return EXIT_SUCCESS;
}
