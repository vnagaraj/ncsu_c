/**
    This component will implement the main function of the conceal program.
    It will be responsible for handling the command-line arguments, reading the
    image and message files, hiding bits of the message in the image and writing
    out the resulting image file.
    @file conceal.c
    @author Vivekanand Ganapathy Nagarajan vganapa
*/

#include <stdlib.h>
#include <stdio.h>
#include "bits.h"
#include "image.h"

/** Length of command line arguments. */
#define PROGRAM_ARGUMENTS 5

/** message file argument */
#define MESSAGE_ARGUMENT 1

/** input image argument */
#define INPUT_IMAGE_ARGUMENT 2

/** output image argument */
#define OUTPUT_IMAGE_ARGUMENT 3

/** no of bits argument */
#define BITS_ARGUMENT 4

/**
  This function reads the message, copies the appropriate bits from source message to
  the destination image struct.
  @param *filename the given message file.
  @param *image the pointer to the Image struct.
  @param bits the number of bits in image to replace.
*/
static void copyMessage(char const *filename, Image *image, int bits)
{
  FILE *fp= fopen(filename, "r");
  if (!fp){
    perror(filename);
    exit(EXIT_FAILURE);
  }
  int ch = fgetc(fp);
  int bits_in_color = 0;
  int color_index = 0;
  int size = image->rows * image->cols * PIXEL_WIDTH;
  while (ch != EOF){
    int bits_in_char = 0;
    while (bits_in_char < BITS_PER_BYTE){
      bool bit = getBit(ch, bits_in_char);
      if (color_index >= size){
        fprintf(stderr, "Too few pixels to conceal message\n");
        exit(EXIT_FAILURE);
      }
      image->color[color_index]= putBit(image->color[color_index], bits_in_color, bit);
      bits_in_color++;
      if (bits_in_color == bits){
        bits_in_color = 0;
        color_index++;
      }
      bits_in_char++;
    }
    ch = fgetc(fp);
  }
  //pad remaing bits of input image colors with 0
  while (color_index < size){
    image->color[color_index]= putBit(image->color[color_index], bits_in_color, false);
    bits_in_color++;
    if (bits_in_color == bits){
      bits_in_color = 0;
      color_index++;
    }
  }
  fclose(fp);
}

/** Starting point of the  conceal program.
  * @param argc The number of arguments.
  * @param *argv[] The array of char pointers, one for each argument
  * @return The exit status.
  */
int main(int argc, char *argv[])
{
  // check for incorrect no of arguments passed
  if (argc != PROGRAM_ARGUMENTS){
    fprintf(stderr, "usage: conceal <input-message> <input-image> <output-image> <bits>\n");
    exit(EXIT_FAILURE);
  }

  int bits = *argv[BITS_ARGUMENT] - '0';
  //bits to be between 1 and 8
  if (bits < 1 || bits > BITS_PER_BYTE){
    fprintf(stderr, "Invalid number of bits\n");
    exit(EXIT_FAILURE);
  }
  //read image
  Image *image = readImage(argv[INPUT_IMAGE_ARGUMENT]);
  //copy message bits to image
  copyMessage(argv[MESSAGE_ARGUMENT], image, bits);
  //write image to output image file provided
  writeImage(image, argv[OUTPUT_IMAGE_ARGUMENT]);
  //free image
  freeImage(image);
  exit(EXIT_SUCCESS);
}
