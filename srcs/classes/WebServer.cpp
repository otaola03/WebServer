#include "WebServer.hpp"
 #include <sys/types.h>
 #include <sys/event.h>
#include "HttpRequest.hpp"

Server*	WebServer::getServerFromPort(int portFd)
{
	for (serverVector::iterator it = serversList.begin(); it != serversList.end(); ++it)
		if ((*it)->containsThisPort(portFd))
			return (*it);
	return NULL;
}

WebServer::WebServer()
{
	intVector	portsNum;
	portsNum.push_back(8080);
	portsNum.push_back(9090);

	intCharMap	errorPages;
	errorPages[404] = "/404.html";

	Location	location;
	locationVector	locations;
	locations.push_back(location);

	Server *server = new Server("server1", "/", portsNum, errorPages, locations);

	intPortMap& serverPorts = server->getPortsList();
	for (intPortMap::iterator it = serverPorts.begin(); it != serverPorts.end(); ++it)
	{
		kq.addPort(it->first);
		ports[it->first] = it->second;
	}

	serversList.push_back(server);
}

WebServer::WebServer(const WebServer& toCopy)
{
	(void)toCopy;
}

WebServer::~WebServer()
{
}

bool	WebServer::isAPort(int fd)
{
	intPortMap::iterator it = ports.find(fd);
	return (it != ports.end());
}

/* static std::string	recvData(int sockfd) */
/* { */
/* 	char buf[1024]; */
/* 	std::string recvData; */
/* 	int numbytes = 1024 - 1; */

/* 	/1* while (numbytes == 1024 - 1) *1/ */
/* 	while (numbytes > 0) */
/* 	{ */
/* 		if ((numbytes = recv(sockfd, buf, sizeof(buf) - 1, 0)) <= 0) */
/* 		{ */
/* 			if (numbytes == 0) */
/* 			{ */
/* 				std::cout << RED << "selectserver: socket "<< sockfd << " hung up\n" << WHITE; */
/* 				return ""; */
/* 			} */
/* 			else if (numbytes == -1) */
/* 			{ */
/*  				/1* perror("recv"); *1/ */
/* 				/1* return ""; *1/ */
/*  				/1* exit(1); *1/ */
/* 			} */
/* 			else if (numbytes == EWOULDBLOCK) */
/* 				return (perror("recv blcok"), ""); */
/* 		} */
/* 		else */
/* 		{ */
/* 			buf[numbytes] = '\0'; */
/* 			std::string stringBuf(buf); */
/* 			recvData += buf; */
/* 		} */
/* 		/1* std::cout << "numbytes: " << numbytes << "\n"; *1/ */
/* 	} */
/* 	/1* std::cout << recvData << "\n\n"; *1/ */
/* 	return recvData; */
/* } */

static std::string	recvData(int sockfd)
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
				return "";
			}
			if (numbytes == EWOULDBLOCK)
				return (std::cout << "HHHHHHHHHHHHHHHHHHHHHHHHHHHHH\n\n\n", "");
		}
		std::string vaca(buf, numbytes);
		i += numbytes;
		recvData += vaca;
	}
	std::cout << "Sockfd: " << sockfd << "\n";
	std::cout << GREEN << "--------------	RECIVE DATA	----------------\n" << WHITE;
	std::cout << recvData << "\n";
	std::cout << GREEN << "----------------------------------------\n\n\n\n" << WHITE;
	return recvData;
}

static bool	sendData(int sockfd, std::string msg)
{
	int numbytes;
	int totalBitsSend = 0;
	int	bitsToSend = msg.length();

	while (totalBitsSend < bitsToSend)
	{
		numbytes = send(sockfd, msg.c_str(), msg.length(), 0);
		if (numbytes == -1)
 			return (perror("recv"), false);
		else
		{
			totalBitsSend += numbytes;
			msg = msg.substr(numbytes, msg.length());
		}
	}
	return true;
}


void	WebServer::serverLoop()
{
	/* char msg[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\nConnection close\r\n\r\nHello world!"; */
	int numEvents;
	int	fd;
	std::string data;
	Server* server;

    while (1) 
	{
		numEvents = kq.listenNewEvents();
		std::cout << RED << "numEvents: " << numEvents << "\n" << WHITE;
		for (int i = 0; i < numEvents; i++)
		{
			fd = kq.getEvSet(i).ident;

			// DISCONNECT
			if (kq.getEvSet(i).flags & EV_EOF)
				close(fd);

			// NEW CLIENT
			else if (isAPort(fd))
			{
				std::cout << CYAN << "--------------	NEW CONNECTION	----------------\n" << WHITE;
				server = getServerFromPort(fd);
				if ((fd = ports[fd]->acceptConnection()) == -1)
					close(fd);
				else
					if (!kq.manageNewConnection(fd))
						close(fd);
				std::cout << CYAN << "----------------------------------------\n\n\n\n" << WHITE;
			}

			// RECIVE DATA ------- MIRA LO DEL MAXBODY SIZE
			else if (kq.getEvSet(i).filter == EVFILT_READ)
			{
				/* data += recvData(fd); */
				clientsData[fd] = recvData(fd);

				/* if (data == "") */
				if (clientsData[fd] == "")
					close(fd);
				else if(!kq.enableWrite(fd))
					close(fd);
			}

			// SEND
			else if (kq.getEvSet(i).filter == EVFILT_WRITE)
			{
				std::cout << "Sockfd: " << fd << "\n";
				std::cout << BLUE << "--------------	SEND DATA	----------------\n" << WHITE;
				/* std::cout << data << "\n"; */
				std::cout << clientsData[fd] << "\n";
				std::cout << BLUE << "----------------------------------------\n\n\n\n" << WHITE;

				/* HttpRequest parser(data); */
				HttpRequest parser(clientsData[fd]);

				/* std::string msg = server->getMessage(parser); */
				HttpResponse response(parser);
				std::string msg = response.getMsg();

				sendData(fd, msg);

				/* if (send(fd, msg.c_str(), msg.length(), 0) == -1) */
				/* 	perror("send"); */
				kq.manageEndedConnection(fd);
				close(fd);
				data = "";
			}
		}
		std::cout << PURPLE << "=============================================\n" << WHITE;
		std::cout << PURPLE << "=============================================\n\n\n" << WHITE;
	}
}



WebServer& WebServer::operator=(const WebServer& toAssign)
{
	(void)toAssign;
	return *this;
}
