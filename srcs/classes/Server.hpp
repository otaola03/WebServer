#ifndef SERVER_HPP

#define SERVER_HPP

#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <cstring>
#include <cstdlib>
#include <arpa/inet.h>
#include <string>
#include <iostream>

#define BACKLOG 5

class Server
{
	private:
		fd_set			master;
		int				listenSocket;

		void	startServer(int port);

	public:
		Server();
		Server(const Server& toCopy);
		~Server();

		int	getListenSocket();
		int	acceptConnection();
		void	closeServer();

		Server& operator=(const Server& toAssign);
};


#endif
