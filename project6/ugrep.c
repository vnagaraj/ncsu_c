/**
  This component contains the main() function. It's responsible for handling
  command-line arguments, using the parser to build a representation for the
  regular expression, reading input lines, using the Pattern representation
  to find places where the pattern matchs part of an input line, and printing
  out the lines with the matches highlighted in red.

  @file ugrep.c
  @author Vivekanand Ganapathy Nagarajan vganapa
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pattern.h"
#include "parse.h"

// On the command line, which argument is the pattern.
#define PAT_ARG 1

// On the command line, which argument is the input file.
#define FILE_ARG 2

// min command line arguments
#define MIN_ARG 2

// max command line arguments
#define MAX_ARG 3

#define SINGLE_QUOTE 39

#define MAX_CHAR_PER_LINE 100

/**
   Helper function to match the line.

   @param *pstr The pointer to the pattern string.
   @param *str The pointer to the line of text.
   @param len The length of the line of text.
   @param (*table) The pointer to a row in the 2d array.
   @param k=-1 checks for line match else checks for char in line match .
   @return true if the line has a matched pattern string.
*/
bool matchedLine(char const *pstr, char const *str, int len,
                    bool (*table)[len + 1], int k)
{
  for (int begin = 0; begin <= len; begin++){
    for (int end = begin; end <= len; end++){
      if (table[begin][end] && k == -1) {
        // Report the matching line.
        return true;
      }
      if (table[begin][end] && k >= begin && k < end) {
        // Report the matching char in line.
        //printf(" begin: %d  end: %d \n", begin, end);
        return true;
      }
    }
  }
  return false;
}

/**
   Helper function to print the line comprising matched text.

   @param *pstr The pointer to the pattern string.
   @param *str The pointer to the line of text.
   @param len The length of the line of text.
   @param (*table) The pointer to a row in the 2d array.
*/
void reportMatchedLine( char const *pstr, char const *str, int len,
                    bool (*table)[ len + 1 ] )
{
    int red = false;
    for (int k = 0; k<len; k++){
      // check for matching char in line
      if (matchedLine(pstr, str, len, table, k)){
        if (!red){
          red = true;
          // Print the matching substring in red
          fprintf(stdout, "\x1b[31m");
        }
      }else{
        if (red){
          red = false;
          // Print the default sequence
          fprintf(stdout, "\x1b[0m");
        }
      }
      // print the character
      printf( "%c", str[ k ] );
    }
    if (red){
        fprintf(stdout, "\x1b[0m");
    }
    printf( "\n" );
}

/**
   Helper function to read char from file/stdin.

   @param *fp The pointer to the given file
   @return char from file if *fp passed else read char from stdin.
*/
static int readChar(FILE *fp)
{
  if (fp){
    return fgetc(fp);
  }
  return fgetc(stdin);
}

/**
   Helper function to read line.

   @param *fp The pointer to the given file.
   @param *line The pointer to the given line.
   @return true if EOF not reached/false otherwise.
*/
static bool readLine(FILE *fp, char *line)
{
  int index = 0;
  int ch = readChar(fp);
  while (ch != '\n' && ch != EOF){
    if (index >= MAX_CHAR_PER_LINE){
      fprintf(stderr, "Input line too long\n");
      exit(EXIT_FAILURE);
    }
    line[index++] = ch;
    ch = readChar(fp);
  }
  line[index] = '\0';
  if (ch == EOF){
    return false;
  }
  return true;
}

/**
   Helper function to match the pattern with file.

   @param *fp The pointer to the given file.
   @param *pstr The pointer to the given pattern string.
*/
static void matchPatternWithFile(FILE *fp, char *pstr)
{
  Pattern *pat = parsePattern(pstr);
  char line[MAX_CHAR_PER_LINE+1];
  while (readLine(fp, line)){
      int len = strlen(line);
      // Zero-filled array to use as our match table.
      bool (*table)[len + 1]  = calloc((len + 1) * (len + 1), sizeof(bool));
      // Find matches for this pattern.
      pat->match(pat, line, len, table);
      if (matchedLine(pstr, line, len, table, -1)){
        // Print them out.
        reportMatchedLine(pstr, line, len, table);
      }
      free(table);
  }
  pat->destroy(pat);
  if (fp){
    fclose(fp);
  }
}

/**
   Entry point for the program, parses command-line arguments, builds
   the pattern and then tests it against lines of input.

   @param argc Number of command-line arguments.
   @param argv List of command-line arguments.
   @return exit status for the program.
*/
int main( int argc, char *argv[] )
{
  if (argc < MIN_ARG || argc > MAX_ARG){
    fprintf(stderr, "usage: ugrep <pattern> [input-file.txt]\n");
    exit(EXIT_FAILURE);
  }
  FILE *fp = NULL;
  if (argc == MAX_ARG){
    fp= fopen(argv[FILE_ARG], "r");
    if (!fp){
      fprintf(stderr, "Can't open input file: %s\n", argv[FILE_ARG]);
      exit(EXIT_FAILURE);
    }
  }
  char *pstr = argv[PAT_ARG];
  matchPatternWithFile(fp, pstr);

  return EXIT_SUCCESS;
}
