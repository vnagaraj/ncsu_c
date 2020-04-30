/** main component. It contais the starting point of the program
    @file sudoku.c
    @author Vivekanand Ganapathy Nagarajan vganapa
*/

#include "validate.h"
#include "solve.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/** Constants for lower and upper size limit**/
#define SIZE_LOW 4
#define SIZE_HIGH 16

/**  Check if given number is a perfect square.
    @param number The number to check.
    @param true, if number is perfect square, false otherwise.
*/
static bool isPerfectSquare(int number)
{
    int iVar;
    float fVar;

    fVar=sqrt((double)number);
    iVar=fVar;

    if (iVar==fVar)
        return true;
    else
        return false;
}

/**  Print the grid values in case the grid can be solved.
    @param size The size of the grid.
    @param grid The puzzle represented as a grid.
*/
static void printGrid(int size, int grid[][size])
{
  for (int row=0; row<size; row++){
    printf(" ");
    for (int col=0; col<size; col++){
      if (col == size-1){
        printf("%2d", grid[row][col]);
      } else{
        printf("%2d ", grid[row][col]);
      }
    }
    printf("\n");
  }
}

/** Starting point of the program.
  * @return The exit status.
  */
int main()
{
  // size of the puzzle
  int size;
  int gridValue;
  // get value from input
  int match = scanf("%d", &size);
  if (match != 1){
     return EXIT_FAILURE;
  }
  // check for invalid input in size
  if (size < SIZE_LOW || size > SIZE_HIGH || !isPerfectSquare(size)){
     printf("Invalid size\n");
     return EXIT_FAILURE;
  }
  int n = sqrt(size);
  bool isSpacePuzzle = false;
  int grid[size][size];
  // input values into the grid
  for (int row=0; row < size; row++){
    for (int col=0; col< size; col++){
      int match = scanf( "%d", &gridValue);
      if (match != 1){
        // non integer
        printf("Invalid input\n");
         return EXIT_FAILURE;
      }
      if (gridValue < 0 || gridValue > size){
        printf("Invalid input\n");
        return EXIT_FAILURE;
      }
      if (gridValue == 0){
        isSpacePuzzle = true;
      }
      grid[row][col] = gridValue;
    }
  }
  //check if puzzle has no spaces, so puzzle is complete and validate if it is valid
  if (!isSpacePuzzle){
    bool success = validateRows(size, grid) && validateCols(size, grid)
                   && validateSquares(n, size, grid);
    if (!success){
      printf("Invalid\n");
      return EXIT_SUCCESS;
    }
    printf("Valid\n");
    return EXIT_SUCCESS;
  }
  //puzzle has blankspaces
  bool success = validateRowsWithSpaces(size, grid) && validateColsWithSpaces(size, grid)
                 && validateSquaresWithSpaces(n, size, grid);
  if (!success){
    printf("Invalid\n");
    return EXIT_SUCCESS;
  }
  success = solve(n, size, grid);
  if (!success){
    printf("Invalid\n");
    return EXIT_SUCCESS;
  }
  printGrid(size, grid);
  return EXIT_SUCCESS;
}
