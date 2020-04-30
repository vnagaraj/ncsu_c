// Do some I/O and math with unsigned types.

#include <stdlib.h>
#include <stdio.h>

/**
   Add up all the digits in the value, x, and return the sum.
*/
unsigned int digitSum( unsigned long x )
{
  unsigned int result = 0;
  while (x > 0){
    result += x%10;
    x = x/10;
  }
  return result;
}

int main()
{
  // Range of values we're supposed to check.
  unsigned long low, high;

  // Target sum we're supposed to look for.
  unsigned int target;



  // Read the range of values from the user.
  int match = scanf("%lu%lu", &low, &high);

  if (match != 2){
    return EXIT_FAILURE;
  }

  // Read the digit sum we're supposed to look for.
  match = scanf("%u",  &target);

  if (match != 1){
    return EXIT_FAILURE;
  }

  // Check all values from low up to high (inclusive), and report the ones
  // that have the given digit sum.
  for (unsigned long i = low; i<= high; i++){
    if (target == digitSum(i)){
      printf("%lu\n", i);
    }
  }

  return EXIT_SUCCESS;
}
