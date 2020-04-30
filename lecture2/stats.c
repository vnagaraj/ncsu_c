/*
   Read a sequence of real numbers and report the minimum, maximum
   and average values.
 */
#include <stdio.h>
#include <stdlib.h>

int main( void )
{
  // Figure out how many values there are.
  int n;
  // ...
  int match = scanf("%d", &n);
  if (match != 1){
    return EXIT_FAILURE;
  }

  // Read in the first value into val.
  double val;
  // ...
  match = scanf("%lf", &val);
  if (match != 1){
    return EXIT_FAILURE;
  }

  // That first value is our tentative min and max value.
  double minValue = val, maxValue = val, total = val;


  // Get the remaining n-1 values and update minValue, maxValue and total
  // based on each one.
  // ...

  for (int i=0; i< n-1; i++){
      match = scanf("%lf", &val);
      if (match != 1){
        return EXIT_FAILURE;
      }
      if (minValue > val){
        minValue = val;
      }
      if (maxValue < val){
        maxValue = val;
      }
      total += val;
  }


  // Report the stats.
  // ...
  printf("Minimum: %8.2f\n", minValue);
  printf("Maximum: %8.2f\n", maxValue);
  printf("Average: %8.2f\n", total/n);

  // Exit successfully.
  // ...
  return EXIT_SUCCESS;
}
