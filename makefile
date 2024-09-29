all: client server

client: client.c
	gcc -o client client.c utility.c -g

server: server.c
	gcc -o server server.c utility.c -g