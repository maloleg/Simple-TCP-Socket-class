all: server client

.PHONY: server
server: tcp_server.c
	$(CC) -std=c89 -Wall -Werror -pedantic tcp_server.c -o server

.PHONY: client
client: tcp_client.c
	$(CC) -std=c89 -Wall -Werror -pedantic tcp_client.c -o client

.PHONY: clean
clean:
	$(RM) server client

