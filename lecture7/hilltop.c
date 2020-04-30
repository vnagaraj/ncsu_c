#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void readGrid( int rows, int cols, int grid[ rows ][ cols ] )
{
  // Add code to read in all the elements of grid from standard input.
  // ...

    for (int row=0; row < rows; row++){
      for (int col=0; col< cols; col++){
        scanf("%d", &grid[row][col]);
      }
    }
}

// Add parameters to to pass a variable-sized array to the following
// function.  It's the same as the previous function, but I want you
// to get a chance to type it in yourself.
void reportMaxima(int rows, int cols, int grid[rows][ cols ])
{
  // Add code to find local maxima and print them out in row major order.
  for (int row=0; row < rows; row++){
    for (int col=0; col < cols; col++){
      bool localMaxima = true;
      int val = grid[row][col];
      //top neighbor
      if (row-1 >=0){
        if (val<= grid[row-1][col]){
          localMaxima = false;
        }
      }
      //bottom neighbor
      if (row+1 < rows){
        if (val <= grid[row+1][col]){
          localMaxima = false;
        }
      }
      //left neighbor
      if (col-1 >=0){
        if (val <= grid[row][col-1]){
          localMaxima = false;
        }

      }
      //right neighbor
      if (col+1 < cols){
        if (val <= grid[row][col+1]){
          localMaxima = false;
        }
      }
      // top left diagonal
      if (row -1 >=0 && col-1 >=0){
        if (val <= grid[row-1][col-1]){
          localMaxima = false;
        }
      }
      // top right diagonal
      if (row -1 >=0 && col+1 < cols){
        if (val <= grid[row-1][col+1]){
          localMaxima = false;
        }
      }
      // bottom left diagonal
      if (row +1 < rows && col-1 >= 0){
        if (val <= grid[row+1][col-1]){
          localMaxima = false;
        }
      }
      // bottom right diagonal
      if (row +1 < rows && col+1 < cols){
        if (val <= grid[row+1][col+1]){
          localMaxima = false;
        }
      }
      if (localMaxima){
        printf("%d %d\n",row, col);
      }
    }
  }
}

int main()
{
  // Add code to read in the grid size from standard input.
  int rows;
  int cols;
  scanf("%d%d", &rows, &cols);


  // Declare a variable-sized array to hold the grid.
  int grid[rows][cols];

  // Call readGrid() to populate the array.
  readGrid(rows, cols, grid);

  // Call reportMaxima() to print out all local maxima.
  reportMaxima(rows, cols, grid);

  return EXIT_SUCCESS;
}
