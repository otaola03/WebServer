#ifndef PORT_HPP

#define PORT_HPP

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
#include <sstream>

#define BACKLOG 5

class Port
{
	private:
		int				sockfd;
		int				port;

	public:
		Port();
		Port(const int port);
		Port(const Port& toCopy);
		~Port();

		int	getSockFd();

		void	activatePort();
		int		acceptConnection();
		void	recvData(void* buf);
		void	sendData(const void* buf);
		void	closePort();

		Port& operator=(const Port& toAssign);
};


#endif
