/** print text box program.
    @file textbox.c
    @author Vivekanand Ganapathy Nagarajan vganapa
  */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/** Width of each line of text in the box. */
#define LINE_WIDTH 60

/** Symbol used to draw the border around the box. */
#define BORDER '*'

/** Read and print a single line of text inside the border.
  * @return False, if no more text in the box.
  */
bool paddedLine()
{
  bool openingBorder = false;
  int ch = getchar();
  if (ch != EOF){
    openingBorder = true;
    putchar(BORDER);
  }
  int width = 0;
  while (ch != EOF) {
      if (ch == '\n' && width >= LINE_WIDTH){
        break;
      } else if (width >= LINE_WIDTH){
         //discard characters exceeding newline
         ch = getchar();
      }
      else if (ch == '\n' && width < LINE_WIDTH){
        putchar(' ');
      } else{
          putchar(ch);
          ch = getchar();
      }
      width++;
  }
  if (openingBorder){
    putchar(BORDER);
    putchar('\n');
  }
  if (ch == EOF){
    return false;
  }
  return true;
}

/** Prints out copies of the character based on count followed by newline.
  * @param ch The character to print.
  * @param count The number of copies.
  */
void lineOfChars( char ch, int count )
{
  for (int i=0; i< count; i++){
    putchar(ch);
  }
  putchar('\n');
}

/** Starting point of the program.
  * @return The exit status.
  */
int main()
{
  lineOfChars(BORDER, LINE_WIDTH+2);
  bool status = paddedLine();
  while (status){
    status = paddedLine();
  }
  lineOfChars(BORDER, LINE_WIDTH+2);
  return EXIT_SUCCESS;
}
