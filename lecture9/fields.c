#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int main()
{
  // Open the input file and print an error message if we're unsuccessful.
  // (the error message is mostly to help you with debugging.  We won't test
  // this behavior).
   FILE *fp = fopen( "input.txt", "r" );
   if ( !fp ) {
     fprintf(stderr, "Can't open input file");
     return EXIT_FAILURE;
   }
   char firstName[12], lastName[12];
   char id[9];
   char email;
   char firstPartPhone[4], secondPartPhone[4], thirdPartPhone[5];

  // Counting input lines, so we can report errors.
  int lineCount = 0;

  // Keep reading input lines until we reach the end-of-file.
  // Write an output line or an error message for each one.
  int match = 0;
  while (match != EOF){
    lineCount++;
    match = fscanf(fp, "%11s%11s%*s %8[^@]", firstName, lastName, id);
    if (match == EOF){
      break;
    }
    fscanf(fp, "%c", &email);
    if (email != '@'){
      fprintf(stderr, "Invalid input, line %d\n", lineCount);
    } else{
      match = fscanf(fp,"%*s %*s %3[0-9]%*[ -]%3[0-9]%*[ -]%4[0-9]", firstPartPhone, secondPartPhone, thirdPartPhone);
      if (match != 3){
        fprintf(stderr, "Invalid input, line %d\n", lineCount);
      } else {
        fprintf(stdout, "%8s %11s %11s (%s)%s-%s\n", id, firstName, lastName, firstPartPhone, secondPartPhone, thirdPartPhone);
      }
    }
    match = fscanf(fp, "%*[^\n]");

  }
  fclose(fp);
  return EXIT_SUCCESS;
}
