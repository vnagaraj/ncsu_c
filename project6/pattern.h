/**
  This is the header file for the pattern component.

  @file pattern.h
  @author Vivekanand Ganapathy Nagarajan vganapa
*/

#ifndef PATTERN_H
#define PATTERN_H

#include <stdbool.h>

//////////////////////////////////////////////////////////////////////
// Superclass for Patterns

/** A short name to use for the Pattern interface. */
typedef struct PatternStruct Pattern;

/**
  Structure used as a superclass/interface for a regular expression
  pattern.  There's a function pointer for an overridable method,
  match(), that reports all the places where this pattern matches a
  given string.  There's also an overridable method for freeing
  resources for the pattern.
*/
struct PatternStruct {
  /** Method for matching this pattern against a given string.  For
      each substring str[ begin ] .. str[ end - 1 ] that matches this
      pattern, this function sets table[ begin ][ end ] to true.

      @param pat pointer to the pattern being matched (essentially, a this
                 pointer).
      @param str input string in which we're finding matches.
      @param len length of str.
      @param table ( len + 1 ) x ( len + 1 ) array of booleans that
                   gets filled in with the substrings where this
                   pattern matches the string.
  */
  void (*match)( Pattern *pat, char const *str, int len, bool (*table)[ len + 1 ] );

  /** Free memory for this pattern, including any subpatterns it contains.
      @param pat pattern to free.
  */
  void (*destroy)( Pattern *pat );
};

/**
  Make a pattern for a single, non-special character, like `a` or `5`.

  @param sym The symbol this pattern is supposed to match.
  @return A dynamically allocated representation for this new pattern.
*/
Pattern *makeLiteralPattern(char sym);

/**
  Make a pattern for the concatenation of patterns p1 and p2.  It
  should match anything that can be broken into two substrings, s1 and
  s2, where the p1 matches the first part (s1) and p2 matches the
  second part (s2).

  @param p1 Subpattern for matching the first part of the string.
  @param p2 Subpattern for matching the second part of the string.
  @return A dynamically allocated representation for this new pattern.
*/
Pattern *makeConcatenationPattern( Pattern *p1, Pattern *p2 );

/**
  Make a pattern for a repetition character, '*', '+', '?'

  @param p pattern for repetition(1 ore more times).
  @param repetition_type type of repetition ('+', '*', '?').
  @return A dynamically allocated representation for this new pattern.
*/
Pattern *makeRepetitionPattern(Pattern *p, int repetition_type);

/**
  Make a pattern for a extended repetition character, {m, n}

  @param p pattern for repetition(m-n times).
  @param start the start range for repetition.
  @param end the end range for repetition.
  @return A dynamically allocated representation for this new pattern.
*/
Pattern *makeExtendedRepetitionPattern(Pattern *p, int start, int end);

/**
  Make a pattern for a the dot character '.'.

  @return A dynamically allocated representation for this new pattern.
*/
Pattern *makeDotPattern();

/**
  Make a pattern for a the start character '$'.

  @param *p The pointer to the pattern to start/end.
  @param anchorType the start/end anchor to match the string.
  @return A dynamically allocated representation for this new pattern.
*/
Pattern *makeAnchorPattern(Pattern *p, int anchorType);

/**
  Make a pattern for the character class.

  @param *sym The pointer to the symbols enclosed in character class.
  @param inverted true if symbols should belong to chracter class.
  @return A dynamically allocated representation for this new pattern.
*/
Pattern *makeCharacterClassPattern(char * sym, bool inverted);

/**
  Make a pattern for the alternation of patterns p1 and p2.  It
  should match either p1 or p2.

  @param p1 Subpattern for matching the first part of the string.
  @param p2 Subpattern for matching the second part of the string.
  @return A dynamically allocated representation for this new pattern.
*/
Pattern *makeAlternationPattern(Pattern *p1, Pattern *p2);

#endif
