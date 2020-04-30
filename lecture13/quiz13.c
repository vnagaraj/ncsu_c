#include <stdio.h>
#include <stdlib.h>

int main()
{
  struct Article {
    int a;
    float b;
    char c[ 20 ];
  } x, *y;

  y = &x;

  printf( "%p\n", &y->b);
  return EXIT_SUCCESS;
}
