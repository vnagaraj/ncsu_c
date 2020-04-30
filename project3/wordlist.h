/** header file for the component that provides support for reading word
    list files, storing and managing the in-memory word list and checking given
    against the list.
    @file document.h
    @author Vivekanand Ganapathy Nagarajan vganapa
*/

#include <stdbool.h>

/** Given a character, returns true if that character is one that can be part of
    a word, false otherwise.
    @param ch The character ch.
    @return true, if character can be part of the word.
*/
bool wordChar(char ch);

/** Given a word from the document, it looks it up in the word list and returns
    one of three values. Either the given word is a proper noun, or it's a
    regular word that's spelled correctly, or it's a word that doesn't match
    anything on the word list.
    @param word The word from the document.
    @return if word is proper noun(1)/regular word(2)/no match(3).
*/
int lookupWord(char const word[]);

/** Given the name of a file, it opens it, reads all the words from it and adds
    any new (unique) words to the in-memory word list. If any error conditions
    occur, this function or a function called by it can print out the right
    error message and terminate the program.
    @param filename The name of the file.
*/
void readWords(char const fileName[]);
