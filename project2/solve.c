/** implementation file for the component that solves a Sudoku puzzle or
    determines that it cannot be solved.
    @file solve.c
    @author Vivekanand Ganapathy Nagarajan
*/

#include "solve.h"
#include <stdbool.h>


/** Get the number of blank spaces in the grid.
  * @param size The size of the grid.
  * @param grid The puzzle represented as a grid.
*/
int getSpaceCount(int size, int grid[size][size])
{
  int count = 0;
  for (int row=0; row<size; row++){
    for (int col=0; col<size; col++){
      if (grid[row][col] == 0){
        count++;
      }
    }
  }
  return count;
}

/** Update the blank space grid to capture position of blank spaces in the grid.
  * @param spaceCount The number of blank spaces in the grid.
  * @param spaceGrid The blank spaces in the grid caputred in space grid.
  * @param size The size of the grid.
  * @param grid The puzzle represented as a grid.
*/
void updateBlankSpaceGrid(int spaceCount, int spaceGrid[spaceCount], int size, int grid[size][size])
{
  int index = 0;
  int posCounter = 0;
  for (int row=0; row<size; row++){
    for (int col=0; col<size; col++){
      posCounter++;
      if (grid[row][col] == 0){
        spaceGrid[index++] = posCounter;
      }
    }
  }
}

/** Compute the row and columnn based on the given position which is
  * stored in the space grid.
  * @param size The size of the grid.
  * @param index The index of the space grid.
  * @param spaceCount The number of blank spaces in the grid.
  * @param spaceGrid The blank spaces in the grid caputred in space grid.
  * @param rowCol The 2 element array which stores the row in index 0 and
                  column in index 1.
*/
void getRowCol(int size, int index, int spaceCount, int spaceGrid[spaceCount], int rowCol[2])
{
  int position = spaceGrid[index];
  int row = (position-1)/size;
  int col = (position-1)%size;
  rowCol[0] = row;
  rowCol[1] = col;
}

/** Private recursive function to solve the Sudoku puzzle represented as grid.
  * @param n The square root of size of the grid.
  * @param size The size of the grid.
  * @param grid The puzzle represented as a grid.
  * @param index The position of the blank space in the spaceGrid.
  * @param spaceCount The number of blank spaces in the grid.
  * @param spaceGrid The position of blank spaces in the puzzle.
  * @return true, if successful in solving the puzzle, false otherwise.
*/
bool recursiveSolve(int n, int size, int grid[size][size], int index, int spaceCount, int spaceGrid[spaceCount])
{
  //base case
  if (index < 0){
    return false;
  }
  if (index >= spaceCount){
    //satisfied all blankSpacces
    return true;
  }
  int rowCol[2];
  getRowCol(size, index, spaceCount, spaceGrid, rowCol);
  int row = rowCol[0];
  int col = rowCol[1];
  int value = grid[row][col];
  if (value == size){
      // values entered at current index do not satisfy condition
      return false;
  }
  grid[row][col] = value+1;
  // there are more blankspaces
  if (validateRowsWithSpaces(size, grid) && validateColsWithSpaces(size, grid) && validateSquaresWithSpaces(n, size, grid)){
      // value does not violate condition, so proceed to next blankspace
      return recursiveSolve(n, size, grid, index+1, spaceCount, spaceGrid);
  } else{
      //enter the next value in the same blankSpace
      bool check =  recursiveSolve(n, size, grid, index, spaceCount, spaceGrid);
      if (check){
        //next value satisfied, so go to next blankspace
        //return recursiveSolve(n, size, grid, index+1, spaceCount, spaceGrid);
        return true;
      }
      //backtrack since all values cannot be satisfied at that index so reset at index
      grid[row][col] = 0;
      //go back to previous blankspace
      return recursiveSolve(n, size, grid, index-1, spaceCount, spaceGrid);
  }
}

/** Solves the Sudoku puzzle represented by the grid.
  * @param n The square root of size of the grid.
  * @param size The size of the grid.
  * @param grid The puzzle represented as a grid.
  * @return true, if successful in solving the puzzle, false otherwise.
*/
bool solve(int n, int size, int grid[size][size])
{
  int spaceCount = getSpaceCount(size, grid);
  int spaceGrid[spaceCount];
  updateBlankSpaceGrid(spaceCount, spaceGrid, size, grid);
  return recursiveSolve(n, size, grid, 0, spaceCount, spaceGrid);
}
