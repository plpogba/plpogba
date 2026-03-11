all: omok_clnt omok_serv
omok_clnt: Omok_client.c
	gcc -g -o omok_clnt Omok_client.c -lcurses

omok_serv: Omok_Server.c
	gcc -g -o omok_serv Omok_Server.c

clean:
	rm -f all omok_clnt omok_serv
