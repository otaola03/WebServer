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

bool	WebServer::acceptNewClient(int fd)
{
	int newfd = ports[fd]->acceptConnection();;
	if (newfd == -1)
		return false;
	Client* newClient = new Client(newfd);
	getServerFromPort(fd)->addClient(newfd, newClient);
	clients[newfd] = newClient;
	kevent(kq, &newClient->getEvSet(), 1, NULL, 0, NULL);
	newClient->add_event(kq, EVFILT_READ);
	newClient->add_event(kq, EVFILT_WRITE);
	return (true);
}

void	WebServer::serverLoop()
{
	char msg[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\nConnection close\r\n\r\nHello world!";
	struct kevent evList[MAX_EVENTS];
	int numEvents;
	/* struct kevent auxEvSet; */
	int	fd;

	struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    while (1) 
	{
		memset(evList, 0, sizeof(evList));
		numEvents = kevent(kq, NULL, 0, evList, 1, NULL);
		for (int i = 0; i < numEvents; i++)
		{
			fd = evList[i].ident;

			// NEW CLIENT
			if (isAPort(fd))
			{
				if (!acceptNewClient(fd))
				{
					clients[fd]->delete_event(kq, EVFILT_READ);
					clients[fd]->delete_event(kq, EVFILT_WRITE);
					close(fd);
					delete clients[fd];
				}
			}

			// DISCONNECT
			else if (evList[i].flags & EV_EOF)
			{
				/* clients[fd]->closeSockFd(kq); */
				
				clients[fd]->delete_event(kq, EVFILT_READ);
				clients[fd]->delete_event(kq, EVFILT_WRITE);
				close(fd);
				delete clients[fd];
			}

			// RECIVE DATA
			else if (evList[i].filter == EVFILT_READ)
			{
				clients[fd]->recvData();
				clients[fd]->enable_event(kq, EVFILT_WRITE);
			}
			else if (evList[i].filter == EVFILT_WRITE)
			{
				/* std::cout << "WRITEEEEE\n"; */
				if (send(fd, msg, sizeof(msg), 0) == -1)
					perror("send");
				clients[fd]->disable_event(kq, EVFILT_WRITE);
				/* clients[fd]->closeSockFd(kq); */
				clients[fd]->delete_event(kq, EVFILT_READ);
				clients[fd]->delete_event(kq, EVFILT_WRITE);
				close(fd);
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

