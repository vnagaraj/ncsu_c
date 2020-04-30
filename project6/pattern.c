/**
  Object-oriented interface for representing and using regular expressions.

  @file pattern.c
  @author Vivekanand Ganapathy Nagarajan vganapa
*/

#include "pattern.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
   Type of pattern used to represent a single, ordinary symbol,
   like 'a' or '5'.
*/
typedef struct {
  // Fields from our superclass.
  void (*match)(Pattern *pat, char const *str, int len, bool (*table)[len + 1]);
  void (*destroy)(Pattern *pat);

  /** Symbol this pattern is supposed to match. */
  char sym;
} LiteralPattern;

// Overridden match() method for a LiteralPattern
static void matchLiteralPattern(Pattern *pat, char const *str, int len,
                                 bool (*table)[len + 1 ])
{
  // Cast down to the struct type pat really points to.
  LiteralPattern *this = (LiteralPattern *) pat;

  // Find all occurreces of the symbol we're supposed to match, and
  // mark them in the match table as matching, 1-character substrings.
  for ( int i = 0; i < len; i++ )
    if ( str[i] == this->sym )
      table[i][i + 1] = true;
}

/**
   A simple function that can be used to free the memory for any
   pattern that doesn't allocate any additional memory other than the
   struct used to represent it (e.g., if it doesn't contain any
   sub-patterns).  It's like a default implementation of the destroy
   method.

   @param pat The pattern to free memory for.
*/
static void destroySimplePattern(Pattern *pat)
{
  // If we don't need fields that are specific to the sub-type, we can just
  // free the block of memory where the object is stored.
  free(pat);
}

/**
  Make a pattern for a single, non-special character, like `a` or `5`.

  @param sym The symbol this pattern is supposed to match.
  @return A dynamically allocated representation for this new pattern.
*/
Pattern *makeLiteralPattern(char sym)
{
  // Make an instance of LiteralPattern, and fill in its state.
  LiteralPattern *this = (LiteralPattern *) malloc( sizeof( LiteralPattern ) );

  this->match = matchLiteralPattern;
  this->destroy = destroySimplePattern;
  this->sym = sym;

  return (Pattern *) this;
}

/**
   Type of pattern used to represent any character specified with '.'
*/
typedef struct {
  // Fields from our superclass.
  void (*match)(Pattern *pat, char const *str, int len, bool (*table)[len + 1]);
  void (*destroy)(Pattern *pat);
} DotPattern;

// Overridden match() method for a DotPattern
static void matchDotPattern(Pattern *pat, char const *str, int len,
                                 bool (*table)[len + 1])
{
  // Find all occurreces of the any symbol and mark them as true.
  // mark them in the match table as matching, 1-character substrings.
  for ( int i = 0; i < len; i++ )
    if (str[i]){
      table[i][i + 1] = true;
    }
}

/**
  Make a pattern for a the dot character '.''

  @return A dynamically allocated representation for this new pattern.
*/
Pattern *makeDotPattern()
{
  // Make an instance of LiteralPattern, and fill in its state.
  DotPattern *this = (DotPattern *) malloc(sizeof(DotPattern));

  this->match = matchDotPattern;
  this->destroy = destroySimplePattern;
  return (Pattern *) this;
}

/**
   Type of pattern used to represent the start patern represented by '^'.
*/
typedef struct {
  // Fields from our superclass.
  void (*match)(Pattern *pat, char const *str, int len, bool (*table)[ len + 1 ] );
  void (*destroy)(Pattern *pat);
  Pattern *p;
  // Anchor type
  enum {START, END} anchor_type;
} AnchorPattern;

// Overridden match() method for a AnchorPattern
static void matchAnchorPattern(Pattern *pat, char const *str, int len,
                                 bool (*table)[len + 1])
{
  // Find all occurreces of the any symbol and mark them as true.
  // mark them in the match table as matching, 1-character substrings.
  // Cast down to the struct type pat really points to.
  AnchorPattern *anchor = (AnchorPattern *) pat;
  if (!anchor->p){
    return;
  }
  bool (*tbl)[len + 1]  = calloc((len + 1) * (len + 1), sizeof(bool));
  anchor->p->match(anchor->p, str, len, tbl);
  for (int begin = 0; begin <= len; begin++ ){
    if (anchor->anchor_type == START){
      if (tbl[0][begin]){
          table[0][begin] = true;
      }
    } else{ //anchor_type == END
      if (tbl[begin][len]){
          table[begin][len] = true;
      }
    }
  }
  free(tbl);
}

/**
  Make a pattern for a the start character '^' / end character '$'

  @param *p The pointer to the pattern to start/end.
  @param anchorType the start/end anchor to match the string.
  @return A dynamically allocated representation for this new pattern.
*/
Pattern *makeAnchorPattern(Pattern *p, int anchorType)
{
  // Make an instance of AnchorPattern, and fill in its state.
  AnchorPattern *this = (AnchorPattern *) malloc( sizeof(AnchorPattern));

  this->match = matchAnchorPattern;
  this->destroy = destroySimplePattern;
  this->p = p;
  this->anchor_type = anchorType;
  return (Pattern *) this;
}

/**
   Type of pattern used to represent the character class represented by
   characters enclosed in '[' , ']'.
*/
typedef struct {
  // Fields from our superclass.
  void (*match)( Pattern *pat, char const *str, int len, bool (*table)[ len + 1 ] );
  void (*destroy)( Pattern *pat );
  // pointer to the the symbols enclosed in character class
  char *sym;
  // check if inverted
  bool inverted;
} CharacterClassPattern;

/**
  Static helper to check if character in string found in symbols enclosed in
  character class.

  @param *str pointer to the string.
  @param *sym pointer to the symbol in the character class.
  @param inverted check if char should belong to the character class.
*/
static bool checkCharacterClass(char ch, char *sym, bool inverted)
{
  while (*sym){
    if (ch == *sym){
      if (inverted){
        return false;
      }
      return true;
    }
    sym++;
  }
  if (inverted){
    return true;
  }
  return false;
}

// Overridden match() method for a CharacterClassPattern
static void matchCharacterClassPattern( Pattern *pat, char const *str, int len,
                                 bool (*table)[len + 1])
{
  CharacterClassPattern *this = (CharacterClassPattern *) pat;

  for ( int i = 0; i < len; i++ ){
    char *ch = this->sym;
    if (checkCharacterClass(str[i], ch, this->inverted)){
      table[i][i + 1] = true;
    }
  }

}

// destroy function used for CharacterClassPattern
static void destroyCharacterClassPattern(Pattern *pat)
{
  CharacterClassPattern *this = (CharacterClassPattern *) pat;
  free(this->sym);
  free(this);
}

/**
  Make a pattern for the character class.

  @param *sym The pointer to the symbols enclosed in character class.
  @param inverted true if symbols should belong to chracter class.
  @return A dynamically allocated representation for this new pattern.
*/
Pattern *makeCharacterClassPattern(char * sym, bool inverted)
{
  // Make an instance of CharacterClassPattern, and fill in its state.
  CharacterClassPattern *this = (CharacterClassPattern *) malloc( sizeof(CharacterClassPattern));

  this->match = matchCharacterClassPattern;
  this->destroy = destroyCharacterClassPattern;
  this->sym = sym;
  this->inverted = inverted;
  return (Pattern *) this;
}

/**
   Type of pattern used to represent the repetition syntax ('+', '*', '?').
*/
typedef struct {
  // Fields from our superclass.
  void (*match)( Pattern *pat, char const *str, int len, bool (*table)[ len + 1 ] );
  void (*destroy)( Pattern *pat );

  // Pointers to the Pattern to repeat.
  Pattern *p;
  // repetition type
  enum {PLUS, STAR, QUESTION} repetition_type;
} RepetitionPattern;

/**
  Function to match the repetition pattern based on syntax.
  @param *pat pointer to the pattern to repeat.
  @param *str pointer to the given string.
  @param len length of the given string.
  @param *table pointer to the 2D array representing subsequence of string.
*/
static void matchRepetitionPattern(Pattern *pat, char const *str, int len,
                                       bool (*table)[len + 1])
{
  RepetitionPattern *rep = (RepetitionPattern *)pat;
  bool (*tbl)[len + 1]  = calloc((len + 1) * (len + 1), sizeof(bool));
  rep->p->match(rep->p, str, len, tbl);
  for (int begin = 0; begin <= len; begin++ ){
    for (int end = begin; end <= len; end++ ) {
      for ( int k = begin; k <= end; k++ ){
        if (tbl[begin][k]){
          table[begin][k] = true;
        }
        if (tbl[k][end]){
          table[k][end] = true;
        }
        if (rep->repetition_type != QUESTION && table[begin][k] && table[k][end]){
          table[begin][end] = true;
        }
      }
    }
  }
  free(tbl);
  if (rep->repetition_type == STAR || rep->repetition_type == QUESTION){
    //indication to skip match during concatenation
    table[0][0] = true;
  }
}

// destroy function used for RepetionPattern
static void destroyRepetitionPattern(Pattern *pat)
{
  // Cast down to the struct type pat really points to.
  RepetitionPattern *this = (RepetitionPattern *) pat;

  // Free  sub-pattern.
  this->p->destroy(this->p);
  // Free the struct representing this object.
  free(this);
}

/**
  Make a pattern for a repetition character, '*', '+', '?'

  @param p pattern for repetition(1 ore more times).
  @param repetition_type type of repetition ('+', '*', '?').
  @return A dynamically allocated representation for this new pattern.
*/
Pattern *makeRepetitionPattern(Pattern *p, int repetition_type)
{
  // Make an instance of RepetitionPattern, and fill in its state.
  RepetitionPattern *this = (RepetitionPattern *) malloc(sizeof(RepetitionPattern));

  this->match = matchRepetitionPattern;
  this->destroy = destroyRepetitionPattern;
  this->p = p;
  this->repetition_type = repetition_type;
  return (Pattern *) this;
}

/**
   Type of pattern used to represent the extended repetition syntax
   of the pattern enclosed in {m, n}.
*/
typedef struct {
  // Fields from our superclass.
  void (*match)( Pattern *pat, char const *str, int len, bool (*table)[ len + 1 ] );
  void (*destroy)( Pattern *pat );

  // Pointers to the Pattern to repeat in start and end range
  Pattern *p;
  //start range;
  int start, end;
} ExtendedRepetitionPattern;

/**
  Function to match the extended repetition pattern based on syntax.
  @param *pat pointer to the pattern to repeat.
  @param *str pointer to the given string.
  @param len length of the given string.
  @param *table pointer to the 2D array representing subsequence of string.
*/
static void matchExtendedRepetitionPattern(Pattern *pat, char const *str, int len,
                                       bool (*table)[len + 1])
{
  ExtendedRepetitionPattern *rep = (ExtendedRepetitionPattern *)pat;
  bool (*tbl)[len + 1]  = calloc((len + 1) * (len + 1), sizeof(bool));
  bool (*init_tbl)[len + 1]  = calloc((len + 1) * (len + 1), sizeof(bool));
  rep->p->match(rep->p, str, len, tbl);
  for (int begin = 0; begin <= len; begin++ ){
    for (int end = begin; end <= len; end++ ) {
      for ( int k = begin; k <= end; k++ ){
        if (tbl[begin][k]){
          init_tbl[begin][k] = true;
        }
        if (tbl[k][end]){
          init_tbl[k][end] = true;
        }
        if (init_tbl[begin][k] && init_tbl[k][end]){
            init_tbl[begin][end] = true;
        }
      }
    }
  }
  free(tbl);
  for (int begin = 0; begin <= len; begin++ ){
    for (int end = begin; end <= len; end++ ) {
        if (init_tbl[begin][end]){
            if (end - begin >= rep->start && (end - begin <= rep->end || rep->end == -1)){
              table[begin][end] = true;
            }
        }
    }
  }
  free(init_tbl);
  if (rep->start == 0){
    //indication to skip match during concatenation
    table[0][0] = true;
  }
}

// destroy function used for ExtendedRepetitionPattern
static void destroyExtendedRepetitionPattern(Pattern *pat)
{
  // Cast down to the struct type pat really points to.
  ExtendedRepetitionPattern *this = (ExtendedRepetitionPattern *) pat;

  // Free  sub-pattern.
  this->p->destroy(this->p);
  // Free the struct representing this object.
  free(this);
}

/**
  Make a pattern for a extended repetition character, {m, n}

  @param p pattern for repetition(m-n times).
  @param start the start range for repetition.
  @param end the end range for repetition.
  @return A dynamically allocated representation for this new pattern.
*/
Pattern *makeExtendedRepetitionPattern(Pattern *p, int start, int end)
{
  // Make an instance of RepetitionPattern, and fill in its state.
  ExtendedRepetitionPattern *this = (ExtendedRepetitionPattern *) malloc(sizeof(RepetitionPattern));

  this->match = matchExtendedRepetitionPattern;
  this->destroy = destroyExtendedRepetitionPattern;
  this->p = p;
  this->start = start;
  this->end = end;
  return (Pattern *) this;
}

/**
   Representation for a type of pattern that contains two sub-patterns
   (e.g., concatenation).  This representation could be used by more
   than one type of pattern, as long as it uses a pointer to a
   different match() function.
*/
typedef struct {
  // Fields from our superclass.
  void (*match)(Pattern *pat, char const *str, int len, bool (*table)[len + 1]);
  void (*destroy)(Pattern *pat);

  // Pointers to the two sub-patterns.
  Pattern *p1, *p2;
} BinaryPattern;

// destroy function used for BinaryPattern
static void destroyBinaryPattern(Pattern *pat)
{
  // Cast down to the struct type pat really points to.
  BinaryPattern *this = (BinaryPattern *) pat;

  // Free our two sub-patterns.
  this->p1->destroy( this->p1 );
  this->p2->destroy( this->p2 );
  // Free the struct representing this object.
  free( this );
}

/**
  Match for the concatenation pattern to concatenate the 2 sub-patterns.

  @param *pat Pointer to the binary pattern.
  @param *const str Pointer to the string to match.
  @param len length of the string.
  @param (*table) pointer to the 2D array representing subsequence of string.
  @return A dynamically allocated representation for this new pattern.
*/

static void matchConcatenationPattern(Pattern *pat, char const *str, int len,
                                       bool (*table)[len + 1])
{
  // Cast down to the struct type pat really points to.
  BinaryPattern *this = (BinaryPattern *) pat;

  // Two more tables for matching sub-expressions.
  bool (*tbl1)[len + 1]  = calloc((len + 1) * (len + 1), sizeof(bool));
  bool (*tbl2)[len + 1]  = calloc((len + 1) * (len + 1), sizeof(bool));

  //  Let our two sub-patterns figure out everywhere they match.
  this->p1->match(this->p1, str, len, tbl1);
  this->p2->match(this->p2, str, len, tbl2);

  // Then, based on their matches, look for all places where their
  // concatenaton matches.  Check all substrings of the input string.
  for (int begin = 0; begin <= len; begin++)
    for (int end = begin; end <= len; end++) {

      // For the [ begin, end ) range, check all places where it could
      // be split into two substrings, the first matching p1 and the second
      // matching p2.
      for ( int k = begin; k <= end; k++ ){
        if (tbl1[begin][k] && tbl2[0][0]){
          table[begin][k] = true;
        }
        if (tbl1[begin][k] && tbl2[k][end]){
          table[begin][end] = true;
        }
      }
    }

  free( tbl1 );
  free( tbl2 );
}

/**
  Make a pattern for the concatenation of patterns p1 and p2.  It
  should match anything that can be broken into two substrings, s1 and
  s2, where the p1 matches the first part (s1) and p2 matches the
  second part (s2).

  @param p1 Subpattern for matching the first part of the string.
  @param p2 Subpattern for matching the second part of the string.
  @return A dynamically allocated representation for this new pattern.
*/
Pattern *makeConcatenationPattern(Pattern *p1, Pattern *p2)
{
  // Make an instance of Binary pattern and fill in its fields.
  BinaryPattern *this = (BinaryPattern *) malloc(sizeof(BinaryPattern));
  this->p1 = p1;
  this->p2 = p2;

  this->match = matchConcatenationPattern;
  this->destroy = destroyBinaryPattern;

  return (Pattern *) this;
}

/**
  Match for the alternation pattern .

  @param *pat Pointer to the pattern for alternation.
  @param *const str Pointer to the string to match.
  @param len length of the string.
  @param (*table) pointer to the 2D array representing subsequence of string.
  @return A dynamically allocated representation for this new pattern.
*/
static void matchAlternationPattern(Pattern *pat, char const *str, int len,
                                       bool (*table)[len + 1])
{
  // Cast down to the struct type pat really points to.
  BinaryPattern *this = (BinaryPattern *) pat;

  // Two more tables for matching sub-expressions.
  bool (*tbl1)[len + 1]  = calloc((len + 1) * (len + 1), sizeof(bool));
  bool (*tbl2)[len + 1]  = calloc((len + 1) * (len + 1), sizeof(bool));

  //  Let our two sub-patterns figure out everywhere they match.
  this->p1->match(this->p1, str, len, tbl1);
  this->p2->match(this->p2, str, len, tbl2);

  // Then, based on their matches, look for all places where their
  // concatenaton matches.  Check all substrings of the input string.
  for (int begin = 0; begin <= len; begin++){
    for (int end = begin; end <= len; end++){
      if (tbl1[begin][end] || tbl2[begin][end]){
          table[begin][end] = true;
        }
    }
  }
  free( tbl1 );
  free( tbl2 );
}

/**
  Make a pattern for the alternation of patterns p1 and p2.  It
  should match either p1 or p2.

  @param p1 Subpattern for matching the first part of the string.
  @param p2 Subpattern for matching the second part of the string.
  @return A dynamically allocated representation for this new pattern.
*/
Pattern *makeAlternationPattern(Pattern *p1, Pattern *p2)
{
  // Make an instance of Binary pattern and fill in its fields.
  BinaryPattern *this = (BinaryPattern *) malloc(sizeof(BinaryPattern));
  this->p1 = p1;
  this->p2 = p2;

  this->match = matchAlternationPattern;
  this->destroy = destroyBinaryPattern;

  return (Pattern *) this;
}
