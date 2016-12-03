myshell:	execute.o parser.o main.o
	$(CC) execute.o parser.o main.o -o $@

execute.o:	execute.c shell.h
	$(CC) -Wall -c $< -o $@

parser.o:	parser.c shell.h
	$(CC) -Wall -c $< -o $@

main.o:	main.c shell.h 
	$(CC) -Wall -c $< -o $@

clean:
	rm -f myshell *.o core *~