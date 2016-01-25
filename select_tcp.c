#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>

#include "trace.h"


#define BUFSIZE 1024

const char* res_xml = "<term_update_info><retcode>0</retcode></term_update_info>";

/*
 * error - wrapper for perror
 */
void error(char *msg) {
	perror(msg);
	exit(1);
}

void setcp(const char* ip, int port,int backlog)
{
	int parentfd; /* parent socket */
	int childfd; /* child socket */
	int portno; /* port to listen on */
	int clientlen; /* byte size of client's address */
	struct sockaddr_in serveraddr; /* server's addr */
	struct sockaddr_in clientaddr; /* client addr */
	char buf[BUFSIZE]; /* message buffer */
	int optval; /* flag value for setsockopt */
	int n; /* message byte size */
	int connectcnt; /* number of connection requests */
	int notdone;
	fd_set readfds;

	/* 
	 * socket: create the parent socket 
	 */
	parentfd = socket(AF_INET, SOCK_STREAM, 0);
	if (parentfd < 0) 
		error("ERROR opening socket");

	/* setsockopt: Handy debugging trick that lets 
	 * us rerun the server immediately after we kill it; 
	 * otherwise we have to wait about 20 secs. 
	 * Eliminates "ERROR on binding: Address already in use" error. 
	 */
	optval = 1;
	setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR, 
			(const void *)&optval , sizeof(int));

	
	if(fcntl(parentfd, F_SETFL, O_NONBLOCK) == -1) {
		error("Error, fcntl");
	}
	/*
	 * build the server's Internet address
	 */
	bzero((char *) &serveraddr, sizeof(serveraddr));

	/* this is an Internet address */
	serveraddr.sin_family = AF_INET;

	/* let the system figure out our IP address */
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	/* this is the port we will listen on */
	serveraddr.sin_port = htons(port);

	/* 
	 * bind: associate the parent socket with a port 
	 */
	if (bind(parentfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) 
		error("ERROR on binding");

	/* 
	 * listen: make this socket ready to accept connection requests 
	 */
	if (listen(parentfd, 5) < 0) /* allow 5 requests to queue up */ 
		error("ERROR on listen");


	/* initialize some things for the main loop */
	clientlen = sizeof(clientaddr);
	connectcnt = 0;
	//fflush(stdout);

	/* 
	 * main loop: wait for connection request or stdin command.
	 *
	 * If connection request, then echo input line 
	 * and close connection. 
	 * If command, then process command.
	 */
	while (1) {

		/* 
		 * select: Has the user typed something to stdin or 
		 * has a connection request arrived?
		 */
		FD_ZERO(&readfds);          /* initialize the fd set */
		FD_SET(parentfd, &readfds); /* add socket fd */
		FD_SET(0, &readfds);        /* add stdin fd (0) */
		if (select(parentfd+1, &readfds, 0, 0, 0) < 0) {
			error("ERROR in select");
		}


		/* if a connection request has arrived, process it */
		if (FD_ISSET(parentfd, &readfds)) {
			/* 
			 * accept: wait for a connection request 
			 */
			childfd = accept(parentfd, (struct sockaddr *) &clientaddr, &clientlen);
			if (childfd < 0) 
				error("ERROR on accept");
			char clientIP[20] = {'\0'};
                        int len = sizeof(clientIP);
			struct sockaddr_in client;
			getpeername(childfd, (struct sockaddr *)&client, &len);
			inet_ntop(AF_INET, &client.sin_addr, clientIP, sizeof(clientIP));

			/* 
			 * read: read input string from the client
			 */
			bzero(buf, BUFSIZE);
			n = read(childfd, buf, BUFSIZE);
			if (n < 0) { 
				error("ERROR reading from socket");
			} else {
			        push_buff_list(buf);	
				TRACELOG("client's ip=%s", clientIP);
				TRACELOG("buf=%s", buf);
			}

			/* 
			 * write: echo the input string back to the client 
			 */
			n = write(childfd, res_xml, strlen(res_xml));
			if (n < 0) 
				error("ERROR writing to socket");
			close(childfd);
		}
	}
	close(parentfd);

}

#if 0
int t_t_main(int argc, char* argv[]) 
{
	select_tcp(NULL, atoi(argv[1]), 5);
	return 0;
}
#endif
