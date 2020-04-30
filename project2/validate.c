/** implementation file for the validate component
    @file validate.c
    @author Vivekanand Ganapathy Nagarajan vganapa
*/

#include "validate.h"
#include <stdbool.h>


/**  Helper method to initialize all the elements in a stack allocated array to false.
    @param size The size of the array.
    @param digitsUsed The array whose elements to be initialized.
*/
static void initializeArray(int size, bool digitsUsed[])
{
  for (int i=0; i<size+1; i++){
    digitsUsed[i] = false;
  }
}

/**  Helper method to validate rows/columns with/without spaces based on
     arguments specified.
    @param size The size of the grid.
    @param grid The grid whose rows to be validated.
    @param isRow To validate every row if true else validate every column.
    @param isSpace To validate row/column with space if true else without space.
    @return true, if criteria of row/column with/without space contain exactly
            one digit in range 1 to size.
*/
static bool validateRowsColumnsWithSpacesOrNot(int size, int grid[][size], bool isRow, bool isSpace)
{
  // 1-1 mapping between value and index
  bool digitsUsed[size+1];
  //initialize array check to false
  initializeArray(size+1, digitsUsed);

  // i - used for outer loop iteration
  // j - used for inner loop iteration
  //iterating through every row to validate row
  for (int i=0; i<size; i++){
     for (int j=0; j<size; j++){
       int value = grid[i][j];
       if (!isRow){
         value = grid[j][i];
       }
       if (!isSpace && value == 0){
         return false;
       }
       if (value < 0 || value > size){
         return false;
       }
       // digit already seen in row/col except for blanks
       if (value != 0 && digitsUsed[value]){
           return false;
        }
        digitsUsed[value] = true;
     }
     // reset array check for next row validation
     initializeArray(size+1, digitsUsed);
  }
  return true;
}

/**  Validate every row in the grid contains exactly one digit in the range of 1 to size,
    false otherwise.
    @param size The size of the grid.
    @param grid The grid whose rows to be validated.
    @return true, if every row in the grid contains exactly one digit in the
            range 1 to size.
*/
bool validateRows(int size, int grid[size][size])
{
  // call helper method for row validation with no spaces
  return validateRowsColumnsWithSpacesOrNot(size, grid, true, false);
}

/** Validate every column in the grid contains exactly one digit in the range of 1 to size,
    false otherwise.
    @param size The size of the grid.
    @param grid The grid whose columns to be validated.
    @return true, if every column in the grid contains exactly one digit in the
            range of 1 to size.
*/
bool validateCols(int size, int grid[size][size])
{
  // call helper method for column validation with no spaces
  return validateRowsColumnsWithSpacesOrNot(size, grid, false, false);
}

/** Private method to validate every n X n square (subgrid) in the grid contains exactly
    one digit in the range 1 to size for space or no more than one digits for non space.
    @param n The no of subgrids.
    @param size The size of the grid.
    @param grid The grid whose square subgrids need to be validated.
    @param isSpace To validate n X n square (subgrid) with space if true else without space.
    @return true,  if every n X n square (subgrid) in the grid based on the
            criteria specified in arguments.
*/
static bool validateSquaresHelper(int n, int size, int grid[][size], bool isSpace)
{
  for (int r1=0; r1<size; r1+=n){
    for (int c1=0; c1<size; c1+=n){
      // 1-1 mapping between value and index
      bool digitsUsed[size+1];
      //initialize array check to false
      initializeArray(size+1, digitsUsed);
      for (int r2=r1; r2<r1+n; r2++){
        for (int c2=c1; c2<c1+n; c2++){
          int value = grid[r2][c2];
          if (!isSpace && value == 0){
            return false;
          }
          if (value < 0 || value > size){
            return false;
          }
          // digit already seen in row/col except for blanks
          if (value != 0 && digitsUsed[value]){
              return false;
           }
           digitsUsed[value] = true;
        }
      }
      // reset digitsUsedArray
      initializeArray(size+1, digitsUsed);
    }
  }
  return true;
}

/** Validate every n X n square (subgrid) in the grid contains exactly one digit
    in the range 1 to size.
    @param n The no of subgrids.
    @param size The size of the grid.
    @param grid The grid whose square subgrids need to be validated.
    @return true,  if every n X n square (subgrid) in the grid contains exactly
            one digit in the range 1 to size.
*/
bool validateSquares(int n, int size, int grid[size][size])
{
  // call helper method to validate subgrids with no space
  return validateSquaresHelper(n, size, grid, false);
}

/** Validate every row in the grid with spaces contains no more than one digit
    in the range of 1 to size.
    @param size The size of the grid.
    @param grid The grid whose rows to be validated.
    @return True, if every row in the grid contains no more than one digit in
            the range 1 to size.
*/
bool validateRowsWithSpaces(int size, int grid[size][size])
{
  // call helper method for row validation with spaces
  return validateRowsColumnsWithSpacesOrNot(size, grid, true, true);
}

/** Validate every column in the grid with spaces contains no more than one digit
    in the range of 1 to size.
    @param size The size of the grid.
    @param grid The grid whose columns to be validated.
    @return True, if every column in the grid contains no more than one digit in
            the range 1 to size.
*/
bool validateColsWithSpaces(int size, int grid[size][size])
{
  // call helper method for column validation with spaces
  return validateRowsColumnsWithSpacesOrNot(size, grid, false, true);
}

/** Validate every n X n square (subgrid) in the grid contains no more than one
    digit in the range 1 to size.
    @param n The no of subgrids.
    @param size The size of the grid.
    @param grid The grid whose square subgrids need to be validated.
    @return true,  if every n X n square (subgrid) in the grid contains no more
            than one digit in the range 1 to size.
*/
bool validateSquaresWithSpaces(int n, int size, int grid[size][size])
{
  // call helper method to validate subgrids with space
  return validateSquaresHelper(n, size, grid, true);
}
