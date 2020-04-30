#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <rpc/des_crypt.h>

// Print a usage message and exit.
static void usage()
{
  fprintf( stderr, "usage: encrypt <key> <input-filename> <output-filename>\n" );
  exit( EXIT_FAILURE );
}

int main( int argc, char *argv[] )
{
  // Check parameters, and open input and output files.
  if ( argc != 4 )
    usage();

  FILE *infile = fopen( argv[ 2 ], "r" );
  if ( ! infile ) {
    fprintf( stderr, "Can't open input file %s\n", argv[ 2 ] );
    usage();
  }

  FILE *outfile = fopen( argv[ 3 ], "wb" );
  if ( ! infile ) {
    fprintf( stderr, "Can't create output file %s\n", argv[ 3 ] );
    usage();
  }

  // Make an 8-byte copy of the key given on the command line.  The
  // one on the command line might be too short, or too long.  Limit
  // it to just the first 8 bytes if it's too long, and pad with zeros
  // ('\0') if it's not long enough.  This will just be an 8-byte
  // array, not a null-terminated string.
  char key[ 8 ];

  strncpy(key, argv[1], sizeof(key));

  // Fill in the parity bits in the key.
  //des_setparity( key );

  // Read the input file 8 bytes at a time, encrypting each block with
  // the key.  If the last block is too short (i.e., if the file isn't
  // a multiple of 8 bytes in length), pad with zeros.
  char block[ 8 ];
  int len;
  while ( ( len = fread( block, 1, 8, infile ) ) ) {
    // Pad any short blocks with zeros.

    //printf("Len before function %d\n", len);
    //printf("Block before %s\n", block);
    while (len < 8){
       block[len++] = '\0';
     }
    // Encrypt this block with the key.  Encrypting each block independently is what
    // the "ecb" in the "ecb_crypt" function name means.
    //printf("Len after function %d\n", len);
    for (int i=0; i < 8; i++){
        printf("%c", block[i]);
    }
    printf("\n");

    //ecb_crypt(key, block, strlen(block), DES_ENCRYPT);

    // Write this encrypted block out to the output file.

    //fwrite(block, 1 ,strlen(block), outfile);
  }

  // Close the two files and exit.
  fclose( infile );
  fclose( outfile );
  return EXIT_SUCCESS;
}
