#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>  // For NULL, although we don't need it.

void swap( int *x, int *y )
{
  int temp = *x;
  *x = *y;
  *y = temp;
}
int main()
{
  int a = 5;
  int *b = &a;
  int c = *b;
  int *d = &c;

  *b += *d;
  printf( "%d\n", a );

  return EXIT_SUCCESS;
}
