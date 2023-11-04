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
	portsNum.push_back(1234);

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
}

void	WebServer::serverLoop()
{
	char msg[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	struct kevent evList[MAX_EVENTS];
	int numEvents;
	int	fd;

    while (1) 
	{
		numEvents = kevent(kq, NULL, 0, evList, MAX_EVENTS, NULL);
		for (int i = 0; i < numEvents; i++)
		{
			fd = evList[i].ident;

			// Accept new client from one port
			if (isAPort(fd))
				acceptNewClient(fd);

			// Client disconnected
			else if (evList[i].flags & EV_EOF)
			{
				clients[fd]->closeSockFd(kq);
				delete clients[fd];
			}

			// Recive data from client
			else if (evList[i].filter == EVFILT_READ)
			{
				if (send(evList[i].ident, msg, sizeof(msg), 0) == -1)
					perror("send");
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

