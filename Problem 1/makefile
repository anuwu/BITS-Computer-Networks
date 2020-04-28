output : client.o server.o helper.o
	gcc -o client client.o helper.o
	gcc -o server server.o helper.o
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
	rm client
	rm server
