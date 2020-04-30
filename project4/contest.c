/** This is the main component. It calls functions in the other components to
    represent the contest and respond to user commands .
    @file contest.c
    @author Vivekanand Ganapathy Nagarajan vganapa
*/

#include <stdlib.h>
#include <stdio.h>
#include "model.h"
#include "command.h"


/** Starting point of the program.
  * @param argc The number of arguments.
  * @param *argv[] The array of char pointers, one for each argument
  * @return The exit status.
  */
int main()
{
  Contest *contest =  makeContest();

  while (true){
    if (!processCommand(contest)){
      printf("Invalid command\n");
    }
  }
  exit(EXIT_SUCCESS);
}
