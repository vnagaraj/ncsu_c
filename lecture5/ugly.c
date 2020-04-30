// Same prime-detection strategy, but without any structured
// looping constructs.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * Report a list of prime values.
 */
int main( void )
{
  // Get the range of values we're supposed to test.
  unsigned int low, high;
  if ( scanf( "%d%d", &low, &high ) != 2 ) {
    printf( "Invalid input\n" );
    exit( EXIT_FAILURE );
  }

  // Write your own, ugly solution, using goto instead of structured looping.

  // ...
  // Test all values from low up to and including high.
  unsigned int v = low;
  unsigned int fact;
  topOfOuterLoop:
    if (v > high){
      goto doneWithOuterLoop;
    }
      fact = 2;
      topOfInnerLoop:
          if (fact * fact > v) {
            printf( "%u\n", v );
            goto doneWithInnerLoop;
          }
          if ( v % fact == 0 ){
            goto doneWithInnerLoop;
          }
          fact++;
          goto topOfInnerLoop;
      doneWithInnerLoop:
  v++;
  goto topOfOuterLoop;
  doneWithOuterLoop:
  return EXIT_SUCCESS;
}
