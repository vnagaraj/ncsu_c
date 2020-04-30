#include <stdio.h>
#include <stdlib.h>



int main()
{
  FILE *fp = fopen( "input2.txt", "r" );
  if ( !fp ) {
    fprintf(stderr, "Can't open input file");
    return EXIT_FAILURE;
  }
  char firstPartPhone[4];
  char secondPartPhone[3];
  char thirdPartPhone[3];

 // Counting input lines, so we can report errors.
 int lineCount = 1;

 // Keep reading input lines until we reach the end-of-file.
 // Write an output line or an error message for each one.
 // ...
 //fscanf( "%s", firstName);
 int match = fscanf(fp, "%3[0-9]%s", firstPartPhone, secondPartPhone );
 printf("%s\n", firstPartPhone);

  return EXIT_SUCCESS;
}
