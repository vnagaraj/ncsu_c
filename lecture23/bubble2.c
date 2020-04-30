// A simple bubble sort

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Swap items pointed to by a and b.
void swap( int *a, int *b )
{
  int temp = *a;
  *a = *b;
  *b = temp;
}

void exitOnError( char const *message )
{
  printf( "%s\n", message );
  exit( EXIT_FAILURE );
}

// Read an input list from the user (there's a
// possible buffer overflow here)
int readList( int *list )
{
  int len;
  if ( scanf( "%d", &len ) != 1 )
    exitOnError( "Can't read input length" );

  for ( int i = 0; i < len; i++ )
    if ( scanf( "%d", list + i ) != 1 )
      exitOnError( "Can't read list item" );

  return len;
}

// Sort the given list using bubble sort.
void sortList( int *list, int len )
{
  // For the early termination check.
  bool done = false;

  // Makmultiple bubble sorting passes throught the list, until
  // it's sorted (done) or until we've made enought to guarantee
  // its sorted.
  for ( int i = 0; !done && i + 1 < len; i++ ) {
    // If we make it through without swapping, we're done.
    done = true;

    // Make a pass through the list, swapping items if the're out of order.
    for ( int j = 0; j < len - i - 1; j++ )
      if ( list[ j ] > list[ j + 1 ] ) {
        int temp = *(list + j);
        *(list + j) = *(list + j + 1) ;
        *(list + j + 1)= temp;
        done = false;
      }
  }
}

// Pint out a (hopefully sorted) list.
void printList( int *list, int len )
{
  for ( int i = 0; i < len; i++ ) {
    printf( "%d", list[ i ] );
    // fencepost behavior.
    if ( i + 1 < len )
      printf( " " );
  }
  printf( "\n" );
}

int main()
{
  int list[ 25000 ];

  int len = readList( list );
  sortList( list, len );
  printList( list, len );

  return 0;
}
