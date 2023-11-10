#include "WebServer.hpp"
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
	intVector	portsNum;
	portsNum.push_back(8080);
	portsNum.push_back(9090);

	intCharMap	errorPages;
	errorPages[404] = "/404.html";

	Location	location;
	locationVector	locations;
	locations.push_back(location);

	Server *server = new Server("server1", "/", portsNum, errorPages, locations);
	std::cout << "fdMax: " << server->fdMax << "\n";

	server->addPortsToPortsList(ports);
	kq = kqueue();
	server->addPortsToKq(kq);

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

void	WebServer::acceptNewClient(int fd)
{
	int newfd = ports[fd]->acceptConnection();;
	if (newfd == -1)
		return ;
	Client* newClient = new Client(newfd);
	getServerFromPort(fd)->addClient(newfd, newClient);
	clients[newfd] = newClient;
	kevent(kq, &newClient->getEvSet(), 1, NULL, 0, NULL);
	newClient->add_event(kq, EVFILT_READ);
	newClient->add_event(kq, EVFILT_WRITE);
}

void	WebServer::serverLoop()
{
	char msg[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	struct kevent evList[MAX_EVENTS];
	int numEvents;
	/* struct kevent auxEvSet; */
	int	fd;

	struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    while (1) 
	{
		std::cout << "Before\n";
		memset(evList, 0, sizeof(evList));
		numEvents = kevent(kq, NULL, 0, evList, 1, NULL);
		std::cout << "After\n";
		for (int i = 0; i < numEvents; i++)
		{
			fd = evList[i].ident;

			// NEW CLIENT
			if (isAPort(fd))
			{
				/* int newfd = ports[fd]->acceptConnection();; */
				/* if (newfd == -1 || newfd < 0) */
				/* 	return ; */

				/* int yes; */
				/* if (setsockopt(newfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) */
				/* { */
				/* 	perror("setsockopt"); */
				/* 	exit(1); */
				/* } */
				/* fcntl(newfd, F_SETFL, O_NONBLOCK); */
				/* EV_SET(&auxEvSet, newfd, EVFILT_READ, EV_ADD, 0, 0, NULL); */
                /* kevent(kq, &auxEvSet, 1, NULL, 0, NULL); */
				acceptNewClient(fd);
			}

			// DISCONNECT
			else if (evList[i].flags & EV_EOF)
			{
				clients[fd]->closeSockFd(kq);
				delete clients[fd];

				/* EV_SET(&auxEvSet, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL); */
                /* kevent(kq, &auxEvSet, 1, NULL, 0, NULL); */
				/* close(fd); */
			}

			// RECIVE DATA
			else if (evList[i].filter == EVFILT_READ)
			{
				/* std::cout << "RECIVE\n"; */
				/* int numbytes; */
				/* char buf[1000]; */
				/* if ((numbytes = recv(fd, buf, sizeof(buf), 0)) <= 0) */
				/* { */
				/* 	if (numbytes == 0) */
				/* 		std::cout << RED << "selectserver: socket "<< fd << " hung up\n" << WHITE; */
				/* 	if (numbytes == -1) */
				/* 	{ */
 						/* perror("recv"); */
 						/* exit(1); */
				/* 	} */
				/* 	if (numbytes == EWOULDBLOCK) */
				/* 		std::cout << "HHHHHHHHHHHHHHHHHHHHHHHHHHHHH\n\n\n"; */
				/* } */
				/* EV_SET(&evList[i], fd, EVFILT_WRITE, EV_ENABLE, 0, 0, NULL); */
				/* kevent(kq, &evList[i], 1, NULL, 0, NULL); */
				clients[fd]->recvData();
				/* clients[fd]->enableWrite(kq); */
				clients[fd]->enable_event(kq, EVFILT_WRITE);
			}
			else if (evList[i].filter == EVFILT_WRITE)
			{
				/* std::cout << "WRITEEEEE\n"; */
				if (send(fd, msg, sizeof(msg), 0) == -1)
					perror("send");
				/* EV_SET(&evList[i], fd, EVFILT_WRITE, EV_ENABLE, 0, 0, NULL); */
				/* kevent(kq, &evList[i], 1, NULL, 0, NULL); */

				/* EV_SET(&auxEvSet, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL); */
                /* kevent(kq, &auxEvSet, 1, NULL, 0, NULL); */
				/* close(fd); */

				/* clients[fd]->disableWrite(kq); */
				/* clients[fd]->delete_event(kq, EVFILT_WRITE); */
				/* clients[fd]->delete_event(kq, EVFILT_READ); */
				clients[fd]->disable_event(kq, EVFILT_WRITE);
				clients[fd]->closeSockFd(kq);
				delete clients[fd];
			}
		}
	}
}



WebServer& WebServer::operator=(const WebServer& toAssign)
{
	(void)toAssign;
	return *this;
}

