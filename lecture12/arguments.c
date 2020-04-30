// Dynamically allocate a 2D array that holds the same set of strings as
// the command line arguments.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printArguments( int rows, int cols, char args[][ cols ] )
{
  // Each row of the args array contains a string.  Print out each of the
  // strings.
   for (int row = 0; row < rows; row++){
     printf("%s\n", args[row]);
   }
}

int findLength(char *ch)
{
  int index = 0;
  while (*ch != '\0'){
      ch += 1;
      index +=1;
  }
  return index;
}

int main( int argc, char *argv[] )
{
  // Figure out the length of the longest command-line argument.
  int longest = 0;

  for (int i=0; i < argc; i++){
    int length = findLength(argv[i]);
    if (length > longest){
      longest = length;
    }
  }

  // Dynamically allocate a 2D array of characers, with one row for every
  // command-line argument and (exactly) enough columns to hold a copy of the
  // widest command-line argument (including the null terminator at the end).
  char (*words)[ longest + 1 ];
  words = (char (*)[longest+1])malloc(argc * (longest+1));


  // Copy each command-line argumetn to a row of this new array.
for (int row =0; row < argc; row++){
  //printf("String: %s\n", argv[row]);
  char * ch = argv[row];
  int col = 0;
  while (*ch != '\0'){
    words[row][col] = *ch;
    ch += 1;
    col += 1;
  }
  words[row][col] = '\0';
  //printf("Word String: %s\n", words[row]);
}

  // Call a function that's expecting the command-line arguments as a 2D array
  // (not as an array of pointers).
  printArguments( argc, longest + 1, words );
  free(words);

  return 0;
}
