#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

// One inch equals this many meters.
#define INCHES_TO_METERS 0.0254

// Representation for for an imperial measurement, as a length in
// inches.  The length is represented as a mixed numeral, with an
// integer number of inches and a simple fraction represented as
// numerator and denominator.  So, in inches, the length would be
// equivalent to inches + numerator / denominator.
typedef struct {
  int inches;
  int numerator;
  int denominator;
} ImperialLength;

// Remptesentation for a type of item in our inventory, with
// a length that's measured in metric or imperial units.
typedef struct {
  // Unique ID for this type of item.
  int sku;

  // Enumeration to indicate whether this is a metric or an imperial
  // measurement.
  enum { metric, imperial } mtype;

  // Length, represented in either imperial units (as an instance of
  // ImperialLength) or metric (as a double).  The mtype field says
  // which element of this union we're using.
  union {
    double mval;
    ImperialLength ival;
  } length;
} Item;

// Print a usage message, and exit unsuccessfully.
static void usage()
{
  fprintf( stderr, "usage: inventory <inventory-file>\n" );
  exit( EXIT_FAILURE );
}

// Complain if the inventory file is bad.
static void invalid()
{
  fprintf( stderr, "Invalid inventory file\n" );
  exit( EXIT_FAILURE );
}

// Comparison function for sorting items.
int itemComp( void const *pA, void const *pB )
{
  // These two pointers are really pointers to items.
  Item const *itemA = (Item *) pA;
  Item const *itemB = (Item *) pB;

  // Compare the length of the items pointed to by itemA and itemB.
  double metrics1;
  double metrics2;
  if (itemA->mtype == metric){
    metrics1 = itemA->length.mval;
  }else{
    //item.length.ival.inches = inches;
    //item.length.ival.numerator = numerator;
    //item.length.ival.denominator = denominator;
    metrics1 = (itemA->length.ival.inches + (double)itemA->length.ival.numerator/(double)itemA->length.ival.denominator) * INCHES_TO_METERS;
  }
  if (itemB->mtype == metric){
    metrics2 = itemB->length.mval;
  } else{
    metrics2 = (itemB->length.ival.inches + (double)itemB->length.ival.numerator/(double)itemB->length.ival.denominator) * INCHES_TO_METERS;
  }
  if (metrics1 < metrics2){
    return -1;
  } else if (metrics1 > metrics2){
    return 1;
  }
  return 0;
}

int main( int argc, char *argv[] )
{
  // Validate input arguments, and open the inventory input file.
  if ( argc != 2 )
    usage();

  FILE *fp = fopen( argv[ 1 ], "r" );
  if ( !fp ) {
    fprintf( stderr, "Can't open input file: %s\n", argv[ 1 ] );
    exit( EXIT_FAILURE );
  }

  // Read the inventory, first the number of items.
  int icount;
  if ( fscanf( fp, "%d", &icount ) != 1 )
    invalid();

  // Then, read all the inventory items.
  Item *ilist = (Item *) malloc( icount * sizeof( Item ) );
  Item item;
  for ( int i = 0; i < icount; i++ ) {
    // Read an item and store it in element i of ilist.
    int sku;
    char mtype;
    if ( fscanf( fp, "%d %c",  &sku, &mtype ) != 2 ){
      invalid();
    }
    item.sku = sku;
    if (mtype == 'm'){
      item.mtype = metric;
      double mval;
      if ( fscanf( fp, "%lf", &mval ) != 1 ){
        invalid();
      }
      item.length.mval = mval;
    } else if (mtype == 'i') {
      item.mtype = imperial;
      int inches;
      int numerator;
      char slash;
      int denominator;
      if ( fscanf( fp, "%d%d %c%d", &inches, &numerator, &slash, &denominator) != 4){
        invalid();
      }
      item.length.ival.inches = inches;
      item.length.ival.numerator = numerator;
      item.length.ival.denominator = denominator;
    } else {
      invalid();
    }
    ilist[i] = item;
  }

  // Sort the items by length.
  qsort( ilist, icount, sizeof( Item ), itemComp );

  // Print out the sorted list, in the same format as the input.
  printf( "%d\n", icount );
  for ( int i = 0; i < icount; i++ ) {
    // Print item i from ilist, with a real-valued length if it's metric, or
    // as a mixed numeral if it's metric.
    Item item = ilist[i];
    printf("%d ",item.sku);
    if (item.mtype == metric){
      //524 m 1.000000
      printf("m %lf\n",item.length.mval);
    } else {
      //207 i 39 1 / 4
      printf("i %d %d / %d\n", item.length.ival.inches, item.length.ival.numerator, item.length.ival.denominator);
    }
  }

  // Free the resources and exit successfully.
  free( ilist );
  fclose( fp );
  return EXIT_SUCCESS;
}
