#include <stdio.h>
#include <stdlib.h>


int main()
{
  short *tbl[] = {
    (short []){ 0 },
    (short []){ 1, 2 },
    (short []){ 3, 4, 5 },
    (short []){ 6, 7, 8, 9 },
  };

  //list[ 3 ][ 2 ] = 'z';
  printf( "%d\n", tbl[ 2 ][ 1 ]  );
   return EXIT_SUCCESS;
}
