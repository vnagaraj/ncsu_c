sudoku: sudoku.o solve.o validate.o
		gcc sudoku.o solve.o validate.o -o sudoku -lm

sudoku.o: sudoku.c solve.h validate.h
		gcc -Wall -std=c99 -c sudoku.c

solve.o:  solve.c solve.h validate.h
		gcc -Wall -std=c99 -c solve.c

validate.o: validate.c validate.h
		gcc -Wall -std=c99 -c validate.c

clean:
		rm -f *.o
		rm -f *.exe
