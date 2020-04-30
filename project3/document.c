/** implementation file for the component that provides support for reading the
    document, using the wordlist component to check the spelling of individual
    words and printing out the document inn the right format.
    @file document.c
    @author Vivekanand Ganapathy Nagarajan vganapa
*/

#include "document.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wordlist.h"
// define MAX line length
#define MAX_LINE_LENGTH 75
// max size of word
#define MAX_WORD_LENGTH 12
//types of words
#define PROPER_NOUN 1
#define REGULAR_WORD 2
#define NO_MATCH 3
//ASCI codes for word separators
#define SPACE 32
#define COMMA 44
#define PERIOD 46
#define OPEN_BRACKET 40
#define CLOSE_BRACKET 41
#define HYPHEN 45
#define DOUBLE_QUOTE 34
#define QUESTION_MARK 63
//ASCI codes for digits
#define ZERO 48
#define NINE 57

/** Private static helper function to check if all characters in the word
    are digits from (0-9)
    @param word to check.
    @return true, if all characters in the word are digits.
*/
static bool isDigitWord(char const word[])
{
  while (*word != '\0'){
    if (*word < ZERO || *word > NINE){
      return false;
    }
    word += 1;
  }
  return true;
}

/** Private static helper function to print word based on its lookup value
    from wordlist.
    @param word The word to print.
    @param chopped Is the word chopped.
*/
static void printWord(char const word[], bool chopped)
{
    if (chopped || isDigitWord(word)){
      fprintf(stdout, "%s", word);
      return;
    }
    int val = lookupWord(word);
    if (val == NO_MATCH){
      //print red text
      fprintf(stdout, "\x1b[31m%s\x1b[0m", word);
    } else if (val == PROPER_NOUN){
      //print blue text
      fprintf(stdout, "\x1b[34m%s\x1b[0m", word);
    }else{
      fprintf(stdout, "%s", word);
    }
}

/** Private static helper to check if the given character is a word separator
    @param ch The character is a word separator.
    @return true if character is a word separator.
*/
static bool checkWordSeparator(int ch)
{
  int wordSeparators[] = {SPACE, COMMA, PERIOD, OPEN_BRACKET, CLOSE_BRACKET,
                          HYPHEN, DOUBLE_QUOTE, QUESTION_MARK};
  for(int i=0; i< sizeof(wordSeparators)/sizeof(wordSeparators[0]); i++){
    if (ch == wordSeparators[i]){
      return true;
    }
  }
  return false;
}

/** Given a line of text from the document, this function finds each word in the
    line, checks it against the word list and prints the line, highlighted as
    necessary. The two flags passed to this function tell it about whether
    the input line had to be truncated when it was read.
    @param line The line of text from the document.
    @param ellipsis If true, print an ellipsis at end of the line.
    @param chopped if true, then last word on the line was chopped and not word.
*/
static void checkLine(char const line[], bool ellipsis, bool chopped)
{
  //variable to iterate through every character
  int charIndex = 0;
  //variable to iterate through every word
  int wordIndex = 0;
  int ch = line[charIndex];
  char word[MAX_LINE_LENGTH+1];
  while (ch != '\n' &&  ch != EOF){
    if (checkWordSeparator(ch) && wordIndex > 0){
        word[wordIndex] = '\0';
        printWord(word, false);
        wordIndex = 0;
        fprintf(stdout, "%c", ch);
    } else if (checkWordSeparator(ch)){
        fprintf(stdout, "%c", ch);
    } else {
        word[wordIndex] = ch;
        wordIndex++;
    }
    charIndex++;
    ch = line[charIndex];
  }
  //last word
  word[wordIndex] = '\0';
  printWord(word, chopped);
  if (ellipsis){
    fprintf(stdout, "...");
  }
  fprintf(stdout, "%c", ch);
}

/** Private helper function to process the line.
    @param *fp The file pointer to process the line.
    @return true if next line is available.
*/
static bool processLine(FILE *fp)
{
  bool ellipsis = false;
  bool chopped = false;
  char line[MAX_LINE_LENGTH+1];
  int charIndex = 0;
  int ch = fgetc(fp);
  while (ch != '\n'  && ch != EOF){
    if (charIndex >=  MAX_LINE_LENGTH){
      ellipsis = true;
      if (ch != ' '){
        chopped = true;
      }
    }else{
      line[charIndex++] = ch;
    }
    ch = fgetc(fp);
  }
  if (charIndex < MAX_LINE_LENGTH){
    line[charIndex] = ch;
  }else{
    line[charIndex] = '\n';
  }
  if (ch == EOF){
    return false;
  }
  checkLine(line, ellipsis, chopped);
  return true;
}

/** It's responsible for reading the input document from the file with the given
    name and printing it out with highlighting (with help from other functions).
    @param fileName the input document from file.
*/
void checkDocument(char const fileName[])
{
  FILE *fp = fopen(fileName, "r");
  if (!fp){
    fprintf(stderr, "Can't open file: %s\n", fileName);
    exit(EXIT_FAILURE);
  }
  bool check = processLine(fp);
  while (check) {
    check = processLine(fp);
  }
  fclose(fp);
}
