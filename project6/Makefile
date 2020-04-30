ugrep: ugrep.o parse.o pattern.o
		gcc ugrep.o parse.o pattern.o -o ugrep

ugrep.o: ugrep.c parse.h pattern.h
		gcc -Wall -std=c99 -g -c ugrep.c

parse.o:  parse.c parse.h pattern.h
		gcc  -DPARSE_H -Wall -std=c99 -g  -c parse.c

pattern.o: pattern.c pattern.h
		gcc  -Wall -std=c99 -g -c pattern.c

clean:
		rm -f *.o
		rm -f *.exe
		rm -f ugrep
