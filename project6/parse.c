/**
  This is the implementation file for the regular expression parser.

  @file parse.c
  @author Vivekanand Ganapathy Nagarajan vganapa
*/

#include "parse.h"
#include "pattern.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define DOT_CHARACTER '.'

#define PLUS_CHARACTER '+'

#define STAR_CHARACTER '*'

#define QUESTION_CHARACTER '?'

#define START_CHARACTER '^'

#define END_CHARACTER '$'

#define OPENING_CHARACTER_CLASS '['

#define CLOSING_CHARACTER_CLASS ']'

#define ALTERNATION_CHARACTER '|'

#define OPEN_PARENTHESIS '('

#define CLOSING_PARENTHESIS ')'

#define OPEN_CURLY '{'

#define CLOSING_CURLY '}'

#define COMMA ','

//in character class
#define INVERTED_CHARACTER '^'

#define DASH_CHARACTER '-'

// repetition types
#define PLUS 0

#define STAR 1

#define QUESTION 2

//anchor types
#define START_ANCHOR 0

#define END_ANCHOR 1

//init capacity of strings in character class
#define INIT_CAP_CHARACTER_CLASS 5

#define MULTIPLY_FACTOR 2

/**
   Return true if the given character is ordinary, if it should just
   match occurrences of itself.  This returns false for metacharacters
   like '*' that control how patterns are matched.

   @param c Character that should be evaluated as ordinary or special.
   @return True if c is not special.
*/
static bool ordinary( char c )
{
  // See if c is on our list of special characters.
  if (strchr( ".^$*?+|()[{", c ) )
    return false;
  return true;
}

/**
  Print the appropriate error message for an invalid pattern and exit
  unsuccessfully.
*/
static void invalidPattern()
{
  fprintf( stderr, "Invalid pattern\n" );
  exit( EXIT_FAILURE );
}

// Forward declaration for a parser function defined below.
static Pattern *parseAlternation(char const *str, int *pos);

/**
   Parse regular expression syntax with the highest precedence,
   individual, ordinary symbols, start and end anchors, character
   classes and patterns surrounded by parentheses.

   @param str The string being parsed.
   @param pos A pass-by-reference value for the location in str being parsed,
              increased as characters from str are parsed.
   @return a dynamically allocated representation of the pattern for the next
           portion of str.
*/
static Pattern *parseAtomicPattern(char const *str, int *pos)
{
  Pattern *p = NULL;
  char sym = str[*pos];
  if (ordinary(sym)) {
    p = makeLiteralPattern(sym);
    (*pos)++;
  } else if (sym == DOT_CHARACTER){
    (*pos)++;
    p = makeDotPattern();
  }else if (sym == OPENING_CHARACTER_CLASS){
    int symbol_length = INIT_CAP_CHARACTER_CLASS;
    char *symbols = (char *)malloc(symbol_length);
    int index = 0;
    (*pos)++;
    bool dash = false;
    char prev_symbol;
    while (str[*pos] && str[*pos] != CLOSING_CHARACTER_CLASS){
      if (dash){
        if (prev_symbol > str[*pos]){
          //check lower and upper bound for range
          invalidPattern();
        }
        while (prev_symbol < str[*pos]){
          symbols[index++] = ++prev_symbol;
          if (index >= symbol_length){
            symbol_length = MULTIPLY_FACTOR * symbol_length;
            symbols = (char *)realloc(symbols, symbol_length);
          }
        }
      }
      if (index != 0 && str[*pos] == DASH_CHARACTER){
        dash = true;
      }else{
        dash = false;
      }
      if (str[*pos] != DASH_CHARACTER){
        prev_symbol = str[*pos];
      }
      symbols[index++] = str[*pos];
      (*pos)++;
      if (index >= symbol_length){
        symbol_length = MULTIPLY_FACTOR * symbol_length;
        symbols = (char *)realloc(symbols, symbol_length);
      }
    }
    if (!str[*pos]){
      //no enclosing closing character class
      if(symbols){
        free(symbols);
      }
      invalidPattern();
    }
    symbols[index] = '\0';
    (*pos)++;
    // check if character class is inverted
    bool inverted = false;
    if (index > 1 && symbols[0] == INVERTED_CHARACTER){
      inverted = true;
    }
    p =  makeCharacterClassPattern(symbols, inverted);
  }


  if (sym == OPEN_PARENTHESIS){
    (*pos)++;
    p =  parseAlternation(str, pos);
  }

  sym = str[*pos];
  if (sym == END_CHARACTER){
    (*pos)++;
    p =  makeAnchorPattern(p, END_ANCHOR);
  }

  if (!p){
    invalidPattern();
  }
  return p; // Just to make the compiler happy.
}

/**
  Utility function to check if the given string is a numeric digit.
  @param *str the pointer to the string.
  @return true if the string is a number.
*/
bool checkNumber(char *str)
{
  while (*str){
    if (!isdigit(*str)){
      return false;
    }
    str +=1;
  }
  return true;
}

/**
   Parse regular expression syntax with the second-highest precedence,
   a pattern, p, optionally followed by one or more repetition syntax like '*' or '+'.
   If there's no repetition syntax, it just returns the pattern object for p.

   @param str The string being parsed.
   @param pos A pass-by-reference value for the location in str being parsed,
              increased as characters from str are parsed.
   @return a dynamically allocated representation of the pattern for the next
           portion of str.
*/
static Pattern *parseRepetition(char const *str, int *pos)
{
  Pattern *p = parseAtomicPattern(str, pos);
  if (str[*pos] == PLUS_CHARACTER || str[*pos] == STAR_CHARACTER ||
                                              str[*pos] == QUESTION_CHARACTER){
      bool plus = (str[*pos] == PLUS_CHARACTER);
      bool star = (str[*pos] == STAR_CHARACTER);
      (*pos)++;
      //make repetition pattern
      if (plus){
        return makeRepetitionPattern(p, PLUS);
      } else if (star){
        return makeRepetitionPattern(p, STAR);
      } else{
        return makeRepetitionPattern(p, QUESTION);
      }
  }
  //check for extended repetition syntax
  if (str[*pos] == OPEN_CURLY){
    (*pos)++;
    int symbol_length = INIT_CAP_CHARACTER_CLASS;
    char *symbols = (char *)malloc(symbol_length);
    int index = 0;
    while (str[*pos] && str[*pos] != CLOSING_CURLY){
      symbols[index++] = str[*pos];
      (*pos)++;
      if (index >= symbol_length){
        symbol_length = MULTIPLY_FACTOR * symbol_length;
        symbols = (char *)realloc(symbols, symbol_length);
      }
    }
    symbols[index] = '\0';
    //no enclosing closing curlyclass or range check
    if (!str[*pos]){
      if(symbols){
        free(symbols);
      }
      invalidPattern();
    }
    char * token = strtok(symbols, ",");
    int start = -1;
    int end = -1;
    while (token) {
      if (checkNumber(token)){
        if (start == -1){
          start = atoi(token);
        }else{
          end = atoi(token);
        }
      }
      token = strtok(NULL, " ");
    }
    // case in which syntax is {,n} then start = 0, end = n
    if (end == -1 && symbol_length > 1 && symbols[0] == COMMA){
      end = start;
      start = 0;
    }
    if(symbols){
      free(symbols);
    }
    if (end != -1 && start > end){
      invalidPattern();
    }
    if (start == -1 && end == -1){
      invalidPattern();
    }
    if (start == 0 && end == 0){
      //valid token not passed
      invalidPattern();
    }
    (*pos)++;
    return makeExtendedRepetitionPattern(p, start, end);
  }
  // no repetition syntax so just returning pattern object p
  return p;
}

/**
   Parse regular expression syntax with the third-highest precedence,
   one pattern, p, (optionally) followed by additional patterns
   (concatenation).  If there are no additional patterns, it just
   returns the pattern object for p.

   @param str The string being parsed.
   @param pos A pass-by-reference value for the location in str being parsed,
              increased as characters from str are parsed.
   @return a dynamically allocated representation of the pattern for the next
           portion of str.
*/
static Pattern *parseConcatenation(char const *str, int *pos)
{
  Pattern *p1 = parseRepetition(str, pos);

  // While there are additional patterns, parse them
  while (str[*pos] && str[*pos] != ALTERNATION_CHARACTER && str[*pos] != CLOSING_PARENTHESIS
                                                && str[*pos] != END_CHARACTER) {
    Pattern *p2 = parseRepetition(str, pos);
    // And build a concatenation pattern to match the sequence.
    p1 = makeConcatenationPattern(p1, p2);
  }

  if (str[*pos] == END_CHARACTER){
    (*pos)++;
    p1 = makeAnchorPattern(p1, END_ANCHOR);
  }

  return p1;
}

/**
   Parse regular expression syntax with the lowest precedence, one
   pattern, p, (optionally) followed by additional patterns separated
   by | (alternation).  If there are no additional patterns, it just
   returns the pattern object for p.

   @param str The string being parsed.
   @param pos A pass-by-reference value for the location in str being
              parsed, increased as characters from str are parsed.
   @return a dynamically allocated representation of the pattern for
   the next portion of str.
*/
static Pattern *parseAlternation(char const *str, int *pos)
{

  Pattern *p1 = parseConcatenation(str, pos);

  while (str[*pos] && str[*pos] == ALTERNATION_CHARACTER){
    (*pos)++;
    Pattern *p2 = parseConcatenation(str, pos);
    p1 = makeAlternationPattern(p1, p2);
  }

  if (str[*pos] && str[*pos] == CLOSING_PARENTHESIS){
    //skip past closing paranthesis
    (*pos)++;
  }
  return p1;
}

/** Parse the given string into Pattern object.

    @param str string cntaining a pattern.
    @return pointer to a representation of the pattern.
*/
Pattern *parsePattern(char const *str)
{
  // Parse the argument into a tree of pattern objects.
  int pos = 0;
  bool start_anchor = false;
  if (str[pos] == START_CHARACTER){
    pos++;
    start_anchor = true;
  }
  Pattern *pat = parseAlternation(str, &pos);

  if (start_anchor){
    pat = makeAnchorPattern(pat, START_ANCHOR);
  }

  // Complain if this didn't consume the whole pattern.
  if (str[pos]){
    invalidPattern();
  }
  return pat;
}
