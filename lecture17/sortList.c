#include <stdio.h>
#include <stdlib.h>

// Put your macro definitions here.  That should be all
// you need to do to complete this exercise.
#define SIZE(name) name##_len

#define CAP(name) name##_cap

#define CHECK_CAP(type, name)  \
      if ( SIZE(list) >= CAP(name) ) { \
            CAP(name) *= 2;\
            name = (type *) realloc( name, CAP(name) * sizeof( type) );\
      }\

#define DECL_LIST(type, name ) \
            int CAP(name) = 5; \
            int SIZE(name) = 0; \
            type *name = (type *) malloc( CAP(name) * sizeof( type ) )

#define SWAP(type, a, b) { \
            type tmp = a; \
            a = b; \
            b = tmp; }

#define FOR( var, limit ) for ( int var = 0; var < limit; var++ )



int main()
{
  // Make a resizable list.
  DECL_LIST( double, list );

  double val;
  while ( scanf( "%lf", &val ) == 1 ) {
    // Grow the list when needed.
    CHECK_CAP( double, list );

    // Add this item to the list
    list[ SIZE( list ) ] = val;
    SIZE( list ) += 1;
  }

  // Bubble-sort the list.
  FOR( i, SIZE( list ) )
    FOR( j, SIZE( list ) - i - 1 )
      if ( list[ j ] > list[ j + 1 ] )
        SWAP( double, list[ j ], list[ j + 1 ] );

  // Print out the resulting, sorted list, one value per line.
  FOR( i, SIZE( list ) )
    printf( "%.2f\n", list[ i ] );

  return EXIT_SUCCESS;
}
