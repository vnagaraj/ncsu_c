/**
    This component will implement the main function of the extract program.
    It will be responsible for handling the command-line arguments and
    extracting the message characters from the input image.
    @file extract.c
    @author Vivekanand Ganapathy Nagarajan vganapa
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "bits.h"
#include "image.h"

/** Length of command line arguments. */
#define PROGRAM_ARGUMENTS 4

/** input image argument */
#define INPUT_IMAGE_ARGUMENT 1

/** message file argument */
#define MESSAGE_ARGUMENT 2

/** no of bits argument */
#define BITS_ARGUMENT 3

/**
  This function extracts the message from the image by using the bits specified
  and copies it to the output filename.
  @param *filename the output message file.
  @param *image the pointer to the Image struct.
  @param bits the number of bits in image to extract.
*/
static void extractMessage(char const *filename, Image *image, int bits)
{
  FILE *fp= fopen(filename, "w");
  if (!fp){
    perror(filename);
    exit(EXIT_FAILURE);
  }
  int size = image->rows * image->cols * PIXEL_WIDTH;
  int color_index = 0;
  int pow_index = 0;
  int asci_code = 0;
  int bit_index = 0;
  while (color_index < size){
    int ch = image->color[color_index];
    //extract the bits from ch
    bool bit = getBit(ch, bit_index++);
    asci_code +=  bit * pow(TWO, pow_index++);

    if (bit_index == bits){
      bit_index = 0;
      //increment color_index
      color_index++;
    }
    if (pow_index == BITS_PER_BYTE){
      pow_index = 0;
      //print out asci character
      if (asci_code == 0){
        //getting 0 byes
        break;
      }
      fputc(asci_code, fp);
      asci_code = 0;
    }
  }
  fclose(fp);
}

/** Starting point of the  extract program.
  * @param argc The number of arguments.
  * @param *argv[] The array of char pointers, one for each argument
  * @return The exit status.
  */
int main(int argc, char *argv[])
{
  // check for incorrect no of arguments passed
  if (argc != PROGRAM_ARGUMENTS){
    fprintf(stderr, "usage: extract <input-image> <output-message> <bits>\n");
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
  //extract message bits from image to output file
  extractMessage(argv[MESSAGE_ARGUMENT], image, bits);
  //free image
  freeImage(image);
  exit(EXIT_SUCCESS);
}
