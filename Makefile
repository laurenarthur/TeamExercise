csubatch: main.o
	gcc -o csubatch main.o -lpthread

main.o:	main.c commandparser.o
	gcc -lpthread -c main.c commandparser.o

commandparser.o: commandparser.c
	gcc -c commandparser.c



clean :
	rm csubatch main.o commandparser.o
