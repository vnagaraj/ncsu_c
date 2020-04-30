/** This is the main component. It calls functions in the other components to
    load the word lists and to spellcheck the document.
    @file spellcheck.c
    @author Vivekanand Ganapathy Nagarajan vganapa
*/

#include <stdlib.h>
#include <stdio.h>
#include "wordlist.h"
#include "document.h"


/** Starting point of the program.
  * @param argc The number of arguments.
  * @param *argv[] The array of char pointers, one for each argument
  * @return The exit status.
  */
int main(int argc, char *argv[])
{
  if (argc <=1){
    fprintf(stderr, "usage: spellcheck <word-file>* <document-file>\n");
    exit(EXIT_FAILURE);
  }
  for (int i=1; i< argc; i++){
    if (i < argc-1){
      //read wordlists
      readWords(argv[i]);
    }else{
      //read input
      checkDocument(argv[i]);
    }
  }
  exit(EXIT_SUCCESS);
}
