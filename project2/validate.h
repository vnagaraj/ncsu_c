/** header file for the validate component
    @file validate.h
    @author Vivekanand Ganapathy Nagarajan vganapa
*/

#include <stdbool.h>

/** Validate every row in the grid contains exactly one digit in the range of 1 to size.
    @param size The size of the grid.
    @param grid The grid whose rows to be validated.
    @return true, if every row in the grid contains exactly one digit in the range 1 to size.
*/
bool validateRows(int size, int grid[size][size]);

/** Validate every column in the grid contains exactly one digit in the range of 1 to size.
    @param size The size of the grid.
    @param grid The grid whose columns to be validated.
    @return true, if every column in the grid contains exactly one digit in the
            range of 1 to size.
*/
bool validateCols(int size, int grid[size][size]);

/** Validate every n X n square (subgrid) in the grid contains exactly one digit
    in the range 1 to size.
    @param n The no of subgrids.
    @param size The size of the grid.
    @param grid The grid whose square subgrids need to be validated.
    @return true,  if every n X n square (subgrid) in the grid contains exactly
            one digit in the range 1 to size.
*/
bool validateSquares(int n, int size, int grid[size][size]);

/** Validate every row in the grid with spaces contains no more than one digit
    in the range of 1 to size.
    @param size The size of the grid.
    @param grid The grid whose rows to be validated.
    @return true, if every row in the grid contains no more than one digit in
            the range 1 to size.
*/
bool validateRowsWithSpaces(int size, int grid[size][size]);

/** Validate every column in the grid with spaces contains no more than one digit
    in the range of 1 to size.
    @param size The size of the grid.
    @param grid The grid whose columns to be validated.
    @return true, if every column in the grid contains no more than one digit
            in the range 1 to size.
*/
bool validateColsWithSpaces(int size, int grid[size][size]);

/** Validate every n X n square (subgrid) in the grid contains no more than one
    digit in the range 1 to size.
    @param n The no of subgrids.
    @param size The size of the grid.
    @param grid The grid whose square subgrids need to be validated.
    @return true,  if every n X n square (subgrid) in the grid contains no more
            than one digit in the range 1 to size.
*/
bool validateSquaresWithSpaces(int n, int size, int grid[size][size]);
