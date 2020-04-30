/** header file for the component that defines the representation for events of
    interest during the contest. The other components use this to keep up
    with the problems, the contestants and the attempts to solve problems.
    @file model.h
    @author Vivekanand Ganapathy Nagarajan vganapa
*/

#include <stdbool.h>

/** Maximum length of a person or problem unique id. */
#define MAX_ID 16

/** Maximum length of a person or problem name. */
#define MAX_NAME 40

#ifndef MODEL_H
#define MODEL_H
/** Representation for a problem in the contest. */
typedef struct {
  /** Unique ID for this problem, containing no whitespace */
  char id[ MAX_ID + 1 ];

  /** Name of this problem (maybe not unique, and maybe with whitespace). */
  char name[ MAX_NAME + 1 ];

  /**successful attempts .*/
  int solutions;

  /**failed attempts. */
  int attempts;

} Problem;

/** Record for an attempt to solve a problem. */
typedef struct {
  /** Problem this attempt was for. */
  Problem *problem;

  /** Was it a successful attempt. */
  bool solved;
} Attempt;

/** Representation for a person or a team in the competition. */
typedef struct {
  /** Unique ID for this person (containing no whitespace). */
  char id[ MAX_ID + 1 ];

  /** Name for this person (maybe not unique and maybe whitespace). */
  char name[ MAX_NAME + 1 ];

  /** Number of attempts. */
  int aCount;

  /** List of all attempts. */
  Attempt *aList;

  /** Capacity of the aList array. */
  int aCap;

  /**solved attempts .*/
  int solved;

  /**penalty totals. */
  int penalty;

} Contestant;

/** Represntation for the whole contest, containing a resizable list of problmes
    and a resizable lit of contestants. */
typedef struct {
  /** Number of problems. */
  int pCount;

  /** List of all problems. */
  Problem **pList;

  /** Capacity of the pList array. */
  int pCap;

  /*/ Number of contestants. */
  int cCount;

  /** List of all the contestants. */
  Contestant **cList;

  /** Capacity of the current cList array. */
  int cCap;
} Contest;

/**
  This dynamically allocates an instance of Problem, initializes its fields
  and returns a pointer to it. It copies the given name and id fields into
  the new struct's fields.
  @param *id the pointer to the given id.
  @param *name the pointer to the given name.
  @return the pointer to the given problem.
*/
Problem *makeProblem(char const *id, char const *name);

/**
  This frees the memory used for the given problem.
  @param *problem the pointer to the given problem.
*/
void freeProblem(Problem *problem);

/**
  This dynamically allocates an instance of Contestant, initializes its fields
  and returns a pointer to it. It copies the given name and id fields into
  the struct's fields, and, since Contestant contains a resizable array,
  this function has to initialize that.
  @param *id the pointer to the id field.
  @param *name the pointer to the name field.
  @return pointer to the instance of Contestant
*/
Contestant *makeContestant(char const *id, char const *name);

/**
  This frees the memory used for the given contestant. This includes freeing
  the memory for the resizable array it contains.
  @param *contestant the pointer to the given contestant.
*/
void freeContestant(Contestant *contestant);

/**
  This dynamically allocates an instance of Contest, initializes its fields
  and returns a pointer to it. Since Contest contains two resizable arrays,
  this function has to initialize both of them.
  @return the pointer to the given Contest.
*/
Contest *makeContest();

/**
  This frees the memory used for the given contest. This includes freeing all
  the problems and contestants on its two lists.
  @param *contest the pointer to the given Contest.
*/
void freeContest(Contest *contest);

/**
  Given a contest and a problem ID, this function returns a pointer to the problem
  with that ID, or NULL if it doesn't exist.
  @param *contest the pointer to the given Contest.
  @param *id the pointer to the given id field.
  @return the pointer to the problem.
*/
Problem *findProblem(Contest *contest, char const *id);

/**
  Given a contest and a contestant ID, this function returns a pointer to the
  contestant with that ID, or NULL if it doesn't exist.
  @param *contest the pointer to the given Contest.
  @param *id the poitner to the given id field.
  @return the pointer to the Contestant.
*/
Contestant *findContestant(Contest *contest, char const *id);

#endif
