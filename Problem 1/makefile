output : client.o server.o
	gcc -o client.exe client.o
	gcc -o server.exe server.o
	make objClean

client.o : client.c
	gcc -c client.c

server.o : server.c
	gcc -c server.c

objClean :
	rm *.o

clean :
	rm *.exe
