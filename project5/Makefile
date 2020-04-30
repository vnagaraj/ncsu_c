all: conceal extract

conceal: conceal.o image.o bits.o
		gcc conceal.o image.o bits.o -o conceal -lm

conceal.o: conceal.c image.h bits.h
		gcc -Wall -std=c99 -c conceal.c

extract: extract.o image.o bits.o
		gcc extract.o image.o bits.o -o extract -lm

extract.o: extract.c image.h
		gcc -Wall -std=c99 -c extract.c

image.o:  image.c image.h
		gcc -D_IMAGE_H_ -Wall -std=c99 -c image.c

bits.o: bits.c bits.h
		gcc -D_BITS_H_ -Wall -std=c99 -c bits.c

clean:
		rm -f *.o
		rm -f *.exe
		rm -f conceal
		rm -f extract
