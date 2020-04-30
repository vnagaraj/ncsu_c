#include <stdio.h>
#include <stdlib.h>

int main()
{
  struct ABC {
    int a, b, c;
  };

  typedef struct ABC type1;
  typedef type1 type2;

    struct ABC v1 = { 5 };
    type1 v2 = { 10 };
    type2 v3 = { 15 };
    v2 = v3;
    v1 = v2;
    v3 = v1;

  return EXIT_SUCCESS;
}
