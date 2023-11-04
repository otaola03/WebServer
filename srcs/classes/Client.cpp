#include "Client.hpp"

Client::Client(int sockfd) : Connection(sockfd)
{
	setSocketNonBlocking(sockfd);
	ev_set();
}

Client::Client(const Client& toCopy)
{
	(void)toCopy;
}

Client::~Client()
{
	/* closeSockFd(); */
}

std::string	Client::recvData()
{
	char buf[MAXDATASIZE];
	std::string recvData;
	int numbytes = MAXDATASIZE;

	while (numbytes == MAXDATASIZE)
	{
		if ((numbytes = recv(sockfd, buf, sizeof(buf), 0)) <= 0)
		{
			if (numbytes == 0)
				std::cout << RED << "selectserver: socket "<< sockfd << " hung up\n" << WHITE;
			if (numbytes == -1)
			{
 				perror("recv");
 				exit(1);
			}
			if (numbytes == EWOULDBLOCK)
				return (std::cout << "HHHHHHHHHHHHHHHHHHHHHHHHHHHHH\n\n\n", "");
		}
		buf[numbytes] = '\0';
		recvData += buf;
	}
	/* std::cout << recvData << "\n\n"; */
	return recvData;
}

Client& Client::operator=(const Client& toAssign)
{
	(void)toAssign;
	return *this;
}


