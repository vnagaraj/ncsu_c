/** implementation file for the component that is responsible for parsing
    and performing user commands. It uses the model component.
    @file command.c
    @author Vivekanand Ganapathy Nagarajan vganapa
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "command.h"
#include "model.h"

//constant keywords for commands
#define QUIT "quit"
#define PROBLEM "problem"
#define CONTESTANT "contestant"
#define ATTEMPT "attempt"
#define SOLVED "solved"
#define LIST "list"
#define PROBLEMS "problems"
#define UNSOLVED "unsolved"
#define CONTESTANTS "contestants"
//constant for increasing the penalty on unsuccessful attempt
#define PENALTY 20
//keyword size array
#define KEYWORD_SIZE 10

/** Helper function to compare 2 strings
    @param *str1 - pointer to first string.
    @param *str2 - pointer to second string.
    @return true if the two strings are equal/false otherwise.
*/
static bool compareStrings(const char * str1, const char * str2)
{
  if (strcmp(str1, str2) == 0){
    return true;
  }
  return false;
}

/** Helper function to resize the array by doubling capacity
    when size >= capacity.
    @param *contest the pointer to the contest.
    @param true if command is "problem"/ else is "contestant".
*/
static void resizeArray(Contest *contest, char type[])
{
  if (compareStrings(type, PROBLEM)){
    //resize array with doubling capacity
    contest->pCap = 2*contest->pCap;
    contest->pList = (Problem **)realloc(contest->pList, contest->pCap * sizeof(Problem *));
  }else if (compareStrings(type, CONTESTANT)){ //contestant
    contest->cCap = 2*contest->cCap;
    contest->cList = (Contestant **)realloc(contest->cList, contest->cCap * sizeof(Contestant *));
  }
}

/** Helper function to remove trailing white spaces from the name field
    @param name the name field .
*/
static void updateName(char name[])
{
  int length = strlen(name);
  int index =0;
  while (name[index]){
    if (!isspace(name[index])){
      break;
    }
    index++;
  }
  if (index > 0){
    //then trailing white spaces
    int count = length - index;
    for (int i=0; i < count; i++){
      name[i] = name[index++];
    }
    name[count] = '\0';
  }
}

/** Helper function to check if id/name of the problem/contestant command is valid
    'id' is valid if length is upto 16 characters
    'name' is valid if length is upto 40 characters
    @param id the problemid/contestant id should be utmost 16 characters.
    @param name of the problem/contestant should be utmost 40 characters.
    @param true if user comm
*/
static bool checkValidCommand(char id[], char name[])
{
  scanf("%17s%41[^\n]", id, name);
  updateName(name);
  //discard user input in rest of line
  bool valid = true;
  int ch = getchar();
  while (ch != '\n'){
    valid = false;
    ch = getchar();
  }
  if (strlen(id) > MAX_ID || strlen(name) > MAX_NAME){
    //invalid command
    return false;
  }
  return valid;
}

static bool quitContest(Contest *contest)
{
  //exit
  freeContest(contest);
  exit(EXIT_SUCCESS);
}

/** Helper function to update the contestant of the given contest
    @param *contest the pointer to the given contest.
    @param contestantid[] the contestantid.
    @param name[] the name.
    @return true if valid command.
*/
static bool updateContestant(Contest *contest, char contestantid[], char name[])
{
  // check if command input length and check if contestant already exists
  if (checkValidCommand(contestantid, name) && !findContestant(contest, contestantid)){
    Contestant *contestant = makeContestant(contestantid, name);
    //update contest
    if (contest->cCount >= contest->cCap -1){
      //resize array with doubling capacity
      resizeArray(contest, CONTESTANT);
    }
    //add contestant
    contest->cList[contest->cCount] = contestant;
    contest->cCount++;
    return true;
  }
  //invalid command
  return false;
}

/** Helper function to update the problem of the given contest
    @param *contest the pointer to the given contest.
    @param problemid[] the problemid.
    @param name[] the name.
    @return true if valid command.
*/
static bool updateProblem(Contest *contest, char problemid[], char name[])
{
  if (checkValidCommand(problemid, name) && !findProblem(contest, problemid)){
    Problem *problem = makeProblem(problemid, name);
    //update contest
    if (contest->pCount >= contest->pCap -1){
      //resize array with doubling capacity
      resizeArray(contest, PROBLEM);
    }
    //add problem
    contest->pList[contest->pCount] = problem;
    contest->pCount++;
    return true;
  }
  //invalid command
  return false;
}

/** Helper function to update the attempts based on whether it is success/fail
    @param *contest the pointer to the given contest.
    @param contestantid[] the contestantid.
    @param problemid[] the problemid.
    @param true if unsuccessful attempt, false otherwise.
    @return true if valid command.
*/
static bool updateAttempt(Contest *contest, char contestantid[], char problemid[], bool attempt)
{
    scanf("%16s%16s", contestantid, problemid);
    Contestant *contestant = findContestant(contest, contestantid);
    Problem *problem = findProblem(contest, problemid);
    if (!contestant || !problem){
      //id dont match
      return false;
    }
    //check if problem is already solved
    for (int i=0; i < contestant->aCount; i++){
      if (compareStrings(contestant->aList[i].problem->id, problem->id)){
        //id match,
        if (contestant->aList[i].solved){
          //can ignore
          return true;
        }
      }
    }
    //resize array with doubling capacity
    if (contestant->aCount >= contestant->aCap -1){
      contestant->aCap = 2*contestant->aCap;
      Attempt *newAttempt = (Attempt *)malloc(contestant->aCap * sizeof(Attempt));
      for (int i=0; i<contestant->aCount; i++){
        newAttempt[i] = contestant->aList[i];
      }
      free(contestant->aList);
      contestant->aList = newAttempt;
  }
  //add new attempt
  contestant->aList[contestant->aCount].problem = problem;
  contestant->aList[contestant->aCount].solved = !attempt;
  contestant->aCount++;

  //updating problem struct
  problem->attempts +=1;
  if (!attempt){
    contestant->solved += 1;
    //compute failed attemp in this problem for the contestant
    for (int i=0; i < contestant->aCount; i++){
      if (strcmp(contestant->aList[i].problem->id, problem->id) == 0){
        //id match,
        if (!contestant->aList[i].solved){
          //can ignore
          contestant->penalty += PENALTY;
        }
      }
    }
    problem->solutions +=1;
  }
  return true;
}

/**
    Test function used for list problems command that always returns true
    @param *problem - pointer to the given problem.
    @param *data - pointer to additional data (contestant) null in this case
    @return true
*/
static bool listAllproblem(Problem *problem, void *data)
{
  return true;
}

/**
    Test function used for list solved command by a given contestant
    @param *problem - pointer to the given problem.
    @param *data - pointer to the given contestant.
    @return true -if contestant has solved the given problem.
*/
static bool listSolvedproblem(Problem *problem, void *data)
{
  Contestant *contestant = data;
  //get attemptlist
  for (int i=0; i< contestant->aCount; i++){
    if (contestant->aList[i].problem == problem && contestant->aList[i].solved){
      return true;
    }
  }
  return false;
}

/**
    Test function used for list solved command by a given contestant
    @param *problem - pointer to the given problem.
    @param *data - pointer to the given contestant.
    @return true -if contestant has not solved the given problem.
*/
static bool listUnSolvedproblem(Problem *problem, void *data)
{
  Contestant *contestant = data;
  //get attemptlist
  for (int i=0; i< contestant->aCount; i++){
    if (contestant->aList[i].problem == problem && contestant->aList[i].solved){
      return false;
    }
  }
  return true;
}

/** Helper function to compare the the 2 problems passed via pointer
    The problems are compared based on more solutions/less attempts/lexicographic
    on problemid.
    @param *aptr pointer to 1st problem for comparison.
    @param *bptr pointer to 2nd problem for comparison.
    @return -1 if problem1 < problem2, 1 if problem1 > problem2, 0 if problems are equal.
*/
static int problemCompare(const void *aptr, const void *bptr)
  {
    Problem const **a = (Problem const **)aptr;
    Problem const **b = (Problem const **)bptr;

    //problem which has more solutions is easier
    if ((*a)->solutions > (*b)->solutions){
      return -1;
    } else if ((*a)->solutions < (*b)->solutions){
      return 1;
    }
    //problem which has less attempts is easier
    if ((*a)->attempts < (*b)->attempts){
      return -1;
    } else if ((*a)->attempts > (*b)->attempts){
      return 1;
    }
    //compare the problem ids
    return strcmp((*a)->id, (*b)->id);
    return 0;
  }

  /** Helper function to compare the the 2 contestants passed via pointer
      The contestants are compared based on more solves/less penalty/lexicographic
      on contestantid.
      @param *aptr pointer to 1st contestant for comparison.
      @param *bptr pointer to 2nd contestant for comparison.
      @return -1 if contestant1 < contestant2, 1 if contestant1 > contestant2,
                0 if contestants are equal.
  */
  static int contestantCompare(const void *aptr, const void *bptr)
    {
      Contestant const **a = (Contestant const **)aptr;
      Contestant const **b = (Contestant const **)bptr;

      //contestant which has more solutions is first
      if ((*a)->solved > (*b)->solved){
        return -1;
      } else if ((*a)->solved < (*b)->solved){
        return 1;
      }
      //contestant which has less penalty is first
      if ((*a)->penalty < (*b)->penalty){
        return -1;
      } else if ((*a)->penalty > (*b)->penalty){
        return 1;
      }
      //compare the contestants ids
      return strcmp((*a)->id, (*b)->id);
      return 0;
    }

/** This function is for listing problems. It's used to implement the list
    problems, list solved and list unsolved commands.
    @param (*test) the pointer to a given function to decide which problem to report.
    @param *data the pointer to any additional data that test function might need to do the job.
*/
static void listProblems(Contest *contest, bool (*test)(Problem *problem, void *data), void *data)
{
  printf("%-16s %-40s %9s %9s\n", "ID", "Name", "Solutions", "Attempts");
  //sort the problem list
  qsort(contest->pList, contest->pCount, sizeof(contest->pList[0]), problemCompare);
  for (int i=0; i< contest->pCount; i++){
    if (test(contest->pList[i], data)){
      printf("%-16s %-40s %9d %9d\n", contest->pList[i]->id, contest->pList[i]->name,
                    contest->pList[i]->solutions, contest->pList[i]->attempts);
    }
  }
}

/** This function is for listing contestants.
    @param *contest the pointer to a given contest.
*/
static void listContestants(Contest *contest)
{
  printf("%-16s %-40s %9s %9s\n", "ID", "Name", "Solved", "Penalty");
  //sort the contestants list
  qsort(contest->cList, contest->cCount, sizeof(contest->cList[0]), contestantCompare);
  for (int i=0; i< contest->cCount; i++){
      printf("%-16s %-40s %9d %9d\n", contest->cList[i]->id, contest->cList[i]->name,
                        contest->cList[i]->solved, contest->cList[i]->penalty);

  }
}

/** This function reads a user command from standard input and performs that command,
    updating or using the given contest instance as necessary. If the user enters
    an invalid command, this function returns false (so the caller can report
    the "Invalid command" message if necessary. To print the command number as
    part of the prompt, this function needs to keep a count of all commands.
    @param *contest the pointer to the given contest.
    @return false incase user enters invalid command.
*/
bool processCommand(Contest *contest)
{
  static int count = 1;
  char keyword[KEYWORD_SIZE+1];
  char problemid[MAX_ID+2];
  char contestantid[MAX_ID+2];
  char name[MAX_NAME+2];
  printf("%d> ", count++);
  int match = scanf("%10s", keyword);
  if (match != 1){
    quitContest(contest);
  }
  printf("\n");
  //quit command
  if (compareStrings(keyword, QUIT)){
     quitContest(contest);
    //problem command
  }else if (compareStrings(keyword, PROBLEM)){
      return updateProblem(contest, problemid, name);
    //contestant command
  }else if (compareStrings(keyword, CONTESTANT)){
      return updateContestant(contest, contestantid, name);
    //unsuccessful attempt
  }else if (compareStrings(keyword, ATTEMPT)){
      return updateAttempt(contest, contestantid, problemid, true);
    //solved
  }else if (compareStrings(keyword, SOLVED)){
      return updateAttempt(contest, contestantid, problemid, false);
    //list commands
  }else if (compareStrings(keyword, LIST)){
      char secondkeyword[KEYWORD_SIZE+2];
      scanf("%16s", secondkeyword);
      if (compareStrings(secondkeyword, PROBLEMS)){
        listProblems(contest, listAllproblem, NULL);
        return true;
      }else if (compareStrings(secondkeyword, SOLVED)){
        scanf("%16s", contestantid);
        Contestant *contestant = findContestant(contest, contestantid);
        if (!contestant){
          return false;
        }
        listProblems(contest, listSolvedproblem, contestant);
        return true;
      }else if (compareStrings(secondkeyword, UNSOLVED)){
        scanf("%16s", contestantid);
        Contestant *contestant = findContestant(contest, contestantid);
        if (!contestant){
          return false;
        }
        listProblems(contest, listUnSolvedproblem, contestant);
        return true;
      }else if (compareStrings(secondkeyword, CONTESTANTS)){
        listContestants(contest);
        return true;
      }else{
        quitContest(contest);
      }
  }else{
      quitContest(contest);
  }
  return true;
}
