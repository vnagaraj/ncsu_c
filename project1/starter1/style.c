/** print a paragraph withrand words program.
    @file style.c
    @author Vivekanand Ganapathy Nagarajan vganapa
  */
#include <stdio.h>
#include <stdlib.h>

/** Line limit specification. */
#define LINE_LIMIT 72

/** Lines of paragraph. */
#define NO_LINES 10

/** Print a random word of specified characters.
  * @param x The number of characters to print.
  */
void printWord( int x )
{
    for ( int i = 0; i < x; i++ ){
      // Print a random lower-case letter.
      printf( "%c", 97 + rand() % 26 );
    }
}

/** Print a line of words.
  */
int printLine() {
	int count = 0, pos = 0, space = 0;
	int len = 1 + rand() % 10;
	// Print a line of words up to a limited length.
	while ( pos + len + space < LINE_LIMIT ) {
		if ( space > 0 ) {
			printf( " " );
                }
		printWord( len );
		pos += len + space;
		len = 1 + rand() % 10;
		space = 1;
		count += 1;
	}
	printf( "\n" );
	return count;
}

/** Print a paragraph of lines.
  * @param n The number of lines to print.
  */
int printParagraph( int n )
{
  int total = 0;
  for ( int i = 0; i < n; i++ ){
    total += printLine();
  }
  return total;
}

/** Starting point of the program.
  * @return The exit status.
  */
int main(){
  int w=printParagraph(NO_LINES);
  printf("Words: %d\n",w);
  return 0;
}
