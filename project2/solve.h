/** header file for the component that solves a Sudoku puzzle or
    determines that it cannot be solved.
    @file solve.h
    @author Vivekanand Ganapathy Nagarajan
*/

#include "validate.h"
#include <stdbool.h>

/** Solves the Sudoku puzzle represented by the grid.
  * @param n The square root of size of the grid.
  * @param size The size of the grid
  * @param grid The puzzle represented as a grid.
  * @return true, if successful in solving the puzzle, false otherwise.
*/
bool solve(int n, int size, int grid[size][size]);
