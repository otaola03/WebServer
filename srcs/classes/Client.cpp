#include "Client.hpp"

Client::Client(int sockfd) : Connection(sockfd)
{
}

Client::Client(const Client& toCopy)
{
	(void)toCopy;
}

Client::~Client()
{
	std::cout << "Client " << sockfd << "closed" << "\n";
	closeSockFd();
}

std::string	Client::recvData()
{
	int numbytes;
	char buf[1024];
	std::string recvData;

	if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) != 0)
	{
		if (numbytes == -1)
		{
 			perror("recv");
 			exit(1);
		}
		recvData += buf;
	}
	return recvData;
}

Client& Client::operator=(const Client& toAssign)
{
	(void)toAssign;
	return *this;
}

