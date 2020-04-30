#include <stdio.h>
#include <stdlib.h>


int main()
{
  int list[] = { 3, 6, 9, 12, 15 };
  int *p = list + 2;
  int *end = list+5;

  *++p = 10;
  for (int *a = list; a < end; a++){
    printf("%d\n", *a);
  }



 return EXIT_SUCCESS;



}
