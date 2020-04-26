output : client.o server.o helper.o
	gcc -o client.exe client.o helper.o
	gcc -o server.exe server.o helper.o
	make objClean

client.o : client.c
	gcc -c client.c

server.o : server.c
	gcc -c server.c

helper.o : helper.c
	gcc -c helper.c

objClean :
	rm *.o

clean :
	rm *.exe
