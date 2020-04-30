contest: contest.o command.o model.o
		gcc contest.o command.o model.o -o contest

contest.o: contest.c command.h model.h
		gcc -Wall -std=c99 -c contest.c

command.o:  command.c command.h model.h
		gcc -Wall -std=c99 -c command.c

model.o: model.c model.h
		gcc -DMODEL_H -Wall -std=c99 -c model.c

clean:
		rm -f *.o
		rm -f *.exe
		rm -f contest
