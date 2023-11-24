#ifndef CLIENT_HPP

#define CLIENT_HPP

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "Connection.hpp"

#define MAXDATASIZE 1024

class Client : public Connection
{
	private:
		struct kevent writeEv;
	public:
		Client(int sockfd);
		Client(const Client& toCopy);
		~Client();

		 std::string	recvData();
		void		sendData(const void* buf);
		/* void		enableWrite(int kq); */
		/* void		disableWrite(int kq); */

		Client& operator=(const Client& toCopy);
};

#endif
