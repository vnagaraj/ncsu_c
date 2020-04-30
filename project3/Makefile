spellcheck: spellcheck.o document.o wordlist.o
		gcc spellcheck.o document.o wordlist.o -o spellcheck

spellcheck.o: spellcheck.c document.h wordlist.h
		gcc -Wall -std=c99 -c spellcheck.c

document.o:  document.c document.h wordlist.h
		gcc -Wall -std=c99 -c document.c

wordlist.o: wordlist.c wordlist.h
		gcc -Wall -std=c99 -c wordlist.c

clean:
		rm -f *.o
		rm -f *.exe
		rm -f spellcheck
