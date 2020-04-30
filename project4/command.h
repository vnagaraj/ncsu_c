/** header file for the component that is responsible for parsing
    and performing user commands. It uses the model component.
    @file command.h
    @author Vivekanand Ganapathy Nagarajan vganapa
*/

#include <stdbool.h>
#include "model.h"

/** This function reads a user command from standard input and performs that command,
    updating or using the given contest instance as necessary. If the user enters
    an invalid command, this function returns false (so the caller can report
    the "Invalid command" message if necessary. To print the command number as
    part of the prompt, this function needs to keep a count of all commands.
    @param *contest the pointer to the given contest.
    @return false incase user enters invalid command.
*/
bool processCommand(Contest *contest);
