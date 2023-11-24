#include "Client.hpp"

Client::Client(int sockfd) : Connection(sockfd)
{
	setSocketNonBlocking(sockfd);
	/* ev_set(); */
	/* add_event(kq, EVFILT_WRITE); */
}

Client::Client(const Client& toCopy)
{
	(void)toCopy;
}

Client::~Client()
{
	/* closeSockFd(); */
}

/* std::string	Client::recvData() */
/* { */
/* 	char buf[MAXDATASIZE]; */
/* 	std::string recvData; */
/* 	int numbytes = MAXDATASIZE; */

/* 	/1* while (numbytes > 0) *1/ */
/* 	while (numbytes == MAXDATASIZE) */
/* 	{ */
/* 		if ((numbytes = recv(sockfd, buf, sizeof(buf), 0)) <= 0) */
/* 		{ */
/* 			if (numbytes == 0) */
/* 				std::cout << RED << "selectserver: socket "<< sockfd << " hung up\n" << WHITE; */
/* 			if (numbytes == -1) */
/* 			{ */
/*  				perror("recv"); */
/*  				/1* exit(1); *1/ */
/* 			} */
/* 			if (numbytes == EWOULDBLOCK) */
/* 				return (perror("recv blcok"), ""); */
/* 		} */
/* 		buf[numbytes] = '\0'; */
/* 		recvData += buf; */
/* 	} */
/* 	/1* std::cout << recvData << "\n\n"; *1/ */
/* 	return recvData; */
/* } */

// std::string	Client::recvData()
// {
// 	char buf[MAXDATASIZE];
// 	std::string recvData;
// 	int numbytes = MAXDATASIZE - 1;

// 	/* while (numbytes > 0) */
// 	while (numbytes == MAXDATASIZE - 1)
// 	{
// 		if ((numbytes = recv(sockfd, buf, sizeof(buf) - 1, 0)) <= 0)
// 		{
// 			if (numbytes == 0)
// 				std::cout << RED << "selectserver: socket "<< sockfd << " hung up\n" << WHITE;
// 				return "";
// 			if (numbytes == -1)
// 			{
//  				perror("recv");
// 				return "";
//  				/* exit(1); */
// 			}
// 			if (numbytes == EWOULDBLOCK)
// 				return (perror("recv blcok"), "X");
// 		}
// 		std::string vaca(buf, numbytes);
// 		recvData += vaca;
// 	}
// 	/* std::cout << recvData << "\n\n"; */
// 	return recvData;
// }

std::string	Client::recvData()
{
	char buf[1025];
	std::string recvData;
	int numbytes = 1024;
	int i = 0;

	while (numbytes == 1024)
	{
		if ((numbytes = recv(sockfd, buf, sizeof(buf) - 1, 0)) <= 0)
		{
			if (numbytes == 0)
				std::cout << "selectserver: socket "<< sockfd << " hung up\n";
			if (numbytes == -1)
			{
 				perror("recv");
 				exit(1);
			}
			if (numbytes == EWOULDBLOCK)
				return (std::cout << "HHHHHHHHHHHHHHHHHHHHHHHHHHHHH\n\n\n", "");
		}
		std::string vaca(buf, numbytes);
		i += numbytes;
		recvData += vaca;
	}
	return recvData;
}

/* void	Client::enableWrite(int kq) */
/* { */
/* 	EV_SET(&evSet, sockfd, EVFILT_WRITE, EV_ADD, 0, 0, NULL); */
/* 	kevent(kq, &evSet, 1, NULL, 0, NULL); */

/* 	/1* std::cout << "sockfd" << getSockFd() << "\n"; *1/ */
/* 	/1* EV_SET(&writeEv, sockfd, EVFILT_WRITE, EV_ADD, 0, 0, NULL); *1/ */
/* 	/1* kevent(kq, &writeEv, 1, NULL, 0, NULL); *1/ */
/* } */

/* void	Client::disableWrite(int kq) */
/* { */
/* 	EV_SET(&evSet, sockfd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL); */
/* 	kevent(kq, &evSet, 1, NULL, 0, NULL); */

/* 	/1* EV_SET(&writeEv, sockfd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL); *1/ */
/* 	/1* kevent(kq, &writeEv, 1, NULL, 0, NULL); *1/ */
/* } */

/* void	Client::disableWrite(int kq) */
/* { */
/* 	EV_SET(&evSet, sockfd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL); */
/* 	kevent(kq, &evSet, 1, NULL, 0, NULL); */

/* 	/1* EV_SET(&writeEv, sockfd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL); *1/ */
/* 	/1* kevent(kq, &writeEv, 1, NULL, 0, NULL); *1/ */
/* } */

Client& Client::operator=(const Client& toAssign)
{
	(void)toAssign;
	return *this;
}
