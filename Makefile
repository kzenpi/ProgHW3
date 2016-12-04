myshell:	execute.o parser.o myshell.o
	$(CC) execute.o parser.o myshell.o -o $@

execute.o:	execute.c shell.h
	$(CC) -Wall -c $< -o $@

parser.o:	parser.c shell.h
	$(CC) -Wall -c $< -o $@

myshell.o:	myshell.c shell.h 
	$(CC) -Wall -c $< -o $@

clean:
	rm -f myshell *.o core *~