/** implementation file for the component that provides support for reading word
    list files, storing and managing the in-memory word list and checking given
    against the list.
    @file document.c
    @author Vivekanand Ganapathy Nagarajan vganapa
*/

#include "wordlist.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// max size of unique words in wordList
#define LIST_SIZE 40000
// max size of word
#define MAX_WORD_LENGTH 12
// max length of line
#define MAX_LINE_LENGTH 75
//types of words
#define PROPER_NOUN 1
#define REGULAR_WORD 2
#define NO_MATCH 3
//ASCI CODES
#define APOSTROPHE 39
#define UPPER_A 65
#define UPPER_Z 90
#define LOWER_A 97
#define LOWER_Z 122
//BIG PRIME
#define ODD_PRIME 31

// global declaration of char pointer
static char words[LIST_SIZE*2][MAX_WORD_LENGTH + 1];
// global index into words array
static int wordCount;

/** Private helper function to compute the index of the given string
    using modular hashing
    @param word The word to compute its index
*/
static int hashString(char const word[])
{
    int hash = 0;
    while (*word){
      hash = (ODD_PRIME * hash + (*word))%LIST_SIZE;
      word += 1;
    }
    return hash;
}

/** Private static helper to check if given word is valid
    word is valid - consist of only letters and apostrophes
    @param word The word to check.
    @return true, if valid word false otherwise.
*/
static bool checkValidWord(char const word[])
{
  while (*word){
    if (!wordChar(*word)){
      return false;
    }
    word += 1;
  }
  return true;
}

/** Private static helper to check if the given word is found in wordlist.
    @param word The word to check with wordlist.
*/
static bool checkWord(char const word[])
{
  int index = hashString(word);
  while (words[index][0]){
    if (strcmp(word, words[index++]) == 0){
      return true;
    }
    if (index == LIST_SIZE*2){
      index = 0;
    }
  }
  return false;
}

/** Private static helper to insert the word into the wordlist
    @param word The word to insert into the wordlist.
*/
static void insertWord(char const word[])
{
  int index = hashString(word);
  while (words[index][0]){
    index++;
    if (index == LIST_SIZE*2){
      index = 0;
    }
  }
  strcpy(words[index], word);
  wordCount++;
}

/** Private helper to check if a given word has 1 or more capital letters
    @param word The word to check.
    @return true, if character in word is a capital letter
*/
static bool checkCapitalLetter(char const word[])
{
  while (*word){
    if (isupper(*word)){
      return true;
    }
    word += 1;
  }
  return false;
}

/** Private static helper to convert a word into all lowercase letters.
    @param word The word.
    @param lowercaseword The lowercase word with all characters in lowercase.
*/
static void  convertToLowerCase(char const word[], char lowercaseword[])
{
  int i = 0;
  while(word[i]) {
     lowercaseword[i] = tolower(word[i]);
     i++;
  }
  lowercaseword[i] = '\0';
}

/** Given a character, returns true if that character is one that can be part of
    a word, false otherwise.
    @param ch The character ch.
    @return true, if character can be part of the word.
*/
bool wordChar(char ch)
{
  if (!(ch == APOSTROPHE || (ch >= UPPER_A && ch <= UPPER_Z) ||
          (ch >= LOWER_A && ch <= LOWER_Z))){
    return false;
  }
  return true;
}

/** Given a word from the document, it looks it up in the word list and returns
    one of three values. Either the given word is a proper noun, or it's a
    regular word that's spelled correctly, or it's a word that doesn't match
    anything on the word list.
    @param word The word from the document.
    @return if word is proper noun(1)/regular word(2)/no match(3).
*/
int lookupWord(char const word[])
{
  //check for proper noun
  int index = hashString(word);
  while (words[index][0]){
    if (strcmp(word, words[index]) == 0 && checkCapitalLetter(words[index])){
      return PROPER_NOUN;
    }
    index++;
    if (index == LIST_SIZE*2){
      index = 0;
    }
  }
  //check for regular word
  char lowercaseword[strlen(word) + 1];
  convertToLowerCase(word, lowercaseword);
  index = hashString(lowercaseword);
  while (words[index][0]){
    if (strcmp(lowercaseword, words[index]) == 0){
      return REGULAR_WORD;;
    }
    index++;
    if (index == LIST_SIZE*2){
      index = 0;
    }
  }
  return NO_MATCH;
}

/** Given the name of a file, it opens it, reads all the words from it and adds
    any new (unique) words to the in-memory word list. If any error conditions
    occur, this function or a function called by it can print out the right
    error message and terminate the program.
    @param filename The name of the file.
*/
void readWords(char const fileName[])
{
  FILE *fp = fopen(fileName, "r");
  if (!fp){
    fprintf(stderr, "Can't open file: %s\n", fileName);
    exit(EXIT_FAILURE);
  }
  char currentWord[MAX_LINE_LENGTH + 1];
  int lineno = 0;
  while (fscanf(fp, "%75s", currentWord) == 1){
    lineno++;
    if (strlen(currentWord) > MAX_WORD_LENGTH || !checkValidWord(currentWord)){
      fprintf(stderr, "Invalid word at %s:%d\n", fileName, lineno);
      exit(EXIT_FAILURE);
    }
    if (!checkWord(currentWord)){
      if (wordCount >= LIST_SIZE){
        fprintf(stderr, "Too many words\n");
        exit(EXIT_FAILURE);
      }
      //add the word to word list
      insertWord(currentWord);
    }
  }
  fclose(fp);
}
