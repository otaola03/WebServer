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

Server*	WebServer::getServerFromClient(int clientFd)
{
	for (serverVector::iterator it = serversList.begin(); it != serversList.end(); ++it)
		if ((*it)->containsThisClient(clientFd))
			return (*it);
	return NULL;
}

WebServer::WebServer()
{
	/* intVector	portsNum; */
	/* portsNum.push_back(8080); */
	/* portsNum.push_back(9090); */

	/* intCharMap	errorPages; */
	/* errorPages[404] = "/404.html"; */

	/* Location	location; */
	/* locationVector	locations; */
	/* locations.push_back(location); */

	/* Server *server = new Server("server1", "/", portsNum, errorPages, locations); */
	/* std::cout << "fdMax: " << server->fdMax << "\n"; */

	/* server->addPortsToPortsList(ports); */
	kq = kqueue();
	/* server->addPortsToKq(kq); */

	/* serversList.push_back(server); */
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

bool	WebServer::acceptNewClient(int fd)
{
	int newfd = ports[fd]->acceptConnection();;
	if (newfd == -1)
		return false;

	Client* newClient = new Client(newfd);

	getServerFromPort(fd)->addClient(newfd, newClient);
	clients[newfd] = newClient;

	kevent(kq, &newClient->getEvSet(), 1, NULL, 0, NULL);

	if (!newClient->add_event(kq, EVFILT_READ))
		return (false);
	if (!newClient->add_event(kq, EVFILT_WRITE))
		return (newClient->delete_event(kq, EVFILT_READ), false);

	return (true);
}

void	WebServer::deleteClient(int fd)
{
	clients[fd]->delete_event(kq, EVFILT_READ);
	clients[fd]->delete_event(kq, EVFILT_WRITE);
	close(fd);
	delete clients[fd];
}

static std::string	recvData(int sockfd)
{
	char buf[1024];
	std::string recvData;
	int numbytes = 1024;

	/* while (numbytes > 0) */
	while (numbytes == MAXDATASIZE)
	{
		if ((numbytes = recv(sockfd, buf, sizeof(buf), 0)) <= 0)
		{
			if (numbytes == 0)
			{
				std::cout << RED << "selectserver: socket "<< sockfd << " hung up\n" << WHITE;
				return "";
			}
			if (numbytes == -1)
			{
 				perror("recv");
				return "";
 				/* exit(1); */
			}
			if (numbytes == EWOULDBLOCK)
				return (perror("recv blcok"), "X");
		}
		buf[numbytes] = '\0';
		recvData += buf;
	}
	/* std::cout << recvData << "\n\n"; */
	return recvData;
}

static bool manage_event(int kq, int fd, struct kevent& evSet, int type, int option)
{
	EV_SET(&evSet, fd, type, option, 0, 0, NULL);
	if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1)
		return false;
	return true;
}

void	WebServer::serverLoop()
{
	char msg[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\nConnection close\r\n\r\nHello world!";
	struct kevent evList[MAX_EVENTS];
	struct kevent evSet;
	int numEvents;
	int	fd;
	int j = 0;
	std::string data;
	Port port(8080);
	port.activatePort();
	fcntl(port.getSockFd(), F_SETFL, O_NONBLOCK);
	manage_event(kq, port.getSockFd(), evSet, EVFILT_READ, EV_ADD);

    while (1) 
	{
		/* memset(evList, 0, sizeof(evList)); */
		std::cout << "before\n";
		numEvents = kevent(kq, NULL, 0, evList, 1, NULL);
		std::cout << "after\n";
		for (int i = 0; i < numEvents; i++)
		{
			fd = evList[i].ident;

			// DISCONNECT
			if (evList[i].flags & EV_EOF)
				close(fd);

			// NEW CLIENT
			else if (fd == port.getSockFd())
			{
				std::cout << "j: " << j++ << "\n";
				if ((fd = port.acceptConnection()) == -1)
				{
					std::cout << "Error accpetin conexion\n";
					close(fd);
				}
				else
					std::cout << "new connecion\n";

				fcntl(fd, F_SETFL, O_NONBLOCK);

				if (!manage_event(kq, fd, evSet, EVFILT_READ, EV_ADD | EV_ENABLE))
					close(fd);

				if (!manage_event(kq, fd, evSet, EVFILT_WRITE, EV_ADD | EV_DISABLE))
					close(fd);
			}


			// RECIVE DATA
			else if (evList[i].filter == EVFILT_READ)
			{
				/* std::cout << "recive data\n"; */
				data = recvData(fd);
				if (data == "")
					close(fd);
				if (!manage_event(kq, fd, evSet, EVFILT_WRITE, EV_ENABLE))
					close(fd);
				std::cout << data << "\n";

			}

			// SEND
			else if (evList[i].filter == EVFILT_WRITE)
			{
				/* std::cout << "send data\n"; */
				if (send(fd, msg, sizeof(msg), 0) == -1)
					close(fd);

				if (!manage_event(kq, fd, evSet, EVFILT_WRITE, EV_DISABLE))
					close(fd);

				if (!manage_event(kq, fd, evSet, EVFILT_READ, EV_DELETE))
					close(fd);

				if (!manage_event(kq, fd, evSet, EVFILT_WRITE, EV_DELETE))
					close(fd);
				close(fd);
			}
		}
	}
}



WebServer& WebServer::operator=(const WebServer& toAssign)
{
	(void)toAssign;
	return *this;
}

