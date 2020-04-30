/**
    Implementation for the image component, with a representation for an Image
    and functions for reading and writing images in PPM format.
    @file image.c
    @author Vivekanand Ganapathy Nagarajan vganapa
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "image.h"

/** PPM file format string */
#define FORMAT "P6"

/** Maximum color value */
#define MAX_COLOR 255

/** Number of intensity values per pixel. */
#define PIXEL_WIDTH 3

/** Length of the header fields. */
#define HEADER_FIELDS_LENGTH 4

/** Representation for image file data */
typedef struct {
  /** number of rows */
  int rows;

  /** pixels per row */
  int cols;

  /** Dynamically allocated pixel data, rows * cols pixels, each with
      3 intensities (RGB), stored in row-major order. */
  unsigned char *color;
} Image;

/**
  Static helper to print invalid image and close file
  @param *fp pointer to the file stream.
*/
static void printInvalidImage(FILE *fp)
{
  fprintf(stderr, "Invalid image file\n");
  fclose(fp);
  exit(EXIT_FAILURE);
}

/**
  Static helper to validate the header information in the PPM image file.
  @param *filename pointer to the image file.
  @param *width pointer to the width dimension.
  @param *height pointer to the height dimension.
  @return the location of the stream after reading input.
*/
static long validateHeaders(char const *filename, int *width, int *height)
{
  FILE *fp = fopen(filename, "r");
  if (!fp){
    perror(filename);
    exit(EXIT_FAILURE);
  }
  char format[sizeof(FORMAT)];
  int maxIntensity;
  int match = fscanf(fp, "%2s\n%d %d\n%d\n", format, width, height, &maxIntensity);
  if (match != HEADER_FIELDS_LENGTH){
    printInvalidImage(fp);
  }
  //checking for P6 in PPM header
  if (strcmp(format, FORMAT) != 0){
    printInvalidImage(fp);
  }
  //checking if width and height are positive dimensions
  if (width < 0 || height < 0){
    printInvalidImage(fp);
  }
  //checking if max intensity matches max color
  if (maxIntensity != MAX_COLOR){
    printInvalidImage(fp);
  }
  int pos = ftell(fp);
  fclose(fp);
  return pos;
}
/**
  This function dynamically allocates an instance of Image and populates it based
  on the given PPM file. If it encounters problems with the format of the image
  file, it prints an appropriate error message and terminates the program.
  @param *filename the given PPM file.
  @return the Image struct.
*/
Image *readImage(char const *filename)
{
  int width;
  int height;
  int pos = validateHeaders(filename, &width, &height);
  //dynamically allocate Image struct
  int size = width * height * PIXEL_WIDTH;
  Image *image = (Image *)malloc(sizeof(Image));
  image->cols = width;
  image->rows = height;
  image->color = (unsigned char *)malloc(size);
  //now read file in binary mode from pos
  FILE *fp = fopen(filename, "rb");
  fseek(fp, pos, SEEK_SET);
  for (int i=0; i < size; i++){
    //printf("Index i %d\n", i);
    int length = fread(&image->color[i], 1, sizeof(image->color[i]), fp);
    if (length != 1){
      printInvalidImage(fp);
    }
    //printf ("%d\n", image->color[i]);
  }
  fclose(fp);
  return image;
}

/**
  This function writes out the given image in Raw PPM format to a file with the
  given name. It will print an error message and terminate the program if the
  given file can't be opened.
  @param *filename pointer to the given PPM file.
  @param *image pointer to the Image struct.
*/
void writeImage(Image *image, char const *filename)
{
  FILE *fp = fopen(filename, "w");
  if (!fp){
    perror(filename);
    exit(EXIT_FAILURE);
  }
  fprintf(fp, "%s\n%d %d\n%d\n", FORMAT, image->cols, image->rows, MAX_COLOR);
  int pos = ftell(fp);
  fclose(fp);
  //open file for writing in binary mode
  fp = fopen(filename, "a");
  fseek(fp, pos, SEEK_SET);
  int size = image->cols * image->rows * PIXEL_WIDTH;
  for (int i=0; i< size; i++){
    fputc(image->color[i], fp);
    //printf("index i %d\n", i);
  }
  fclose(fp);
}

/**
  This function frees all the memory associated with an image representation,
  including the pixel data and the Image struct itself.
  @param *image pointer to the Image struct.
*/
void freeImage(Image *image)
{
  free(image->color);
  free(image);
}
