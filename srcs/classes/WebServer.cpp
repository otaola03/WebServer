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

void	WebServer::serverLoop()
{
	char msg[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	struct kevent evList[MAX_EVENTS];
	struct kevent evSet;
	int numEvents;
	int newfd;

    while (1) 
	{
		numEvents = kevent(kq, NULL, 0, evList, MAX_EVENTS, NULL);
		for (int i = 0; i < numEvents; i++)
		{
			if (isAPort(evList[i].ident))
			{
				/* std::cout << GREEN << "New connection\n" << WHITE; */
				newfd = ports[evList[i].ident]->acceptConnection();
				if (newfd == -1)
					continue;
				Client* newClient = new Client(newfd);
				getServerFromPort(evList[i].ident)->addClient(newfd, newClient);
				clients[newfd] = newClient;
				EV_SET(&evSet, newfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
                kevent(kq, &evSet, 1, NULL, 0, NULL);
			}
			else if (evList[i].flags & EV_EOF)
			{
				/* std::cout << RED << "disconnect\n" << WHITE; */
				int fd = evList[i].ident;
				std::cout << "Disconnected: " << fd << "\n";
                EV_SET(&evSet, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                kevent(kq, &evSet, 1, NULL, 0, NULL);
				close(fd);
			}
			else if (evList[i].filter == EVFILT_READ)
			{
				/* std::cout << CYAN << "recive data\n" << WHITE; */
				if (send(evList[i].ident, msg, sizeof(msg), 0) == -1)
					perror("send");
				close(evList[i].ident);
			}
		}
	}
}



WebServer& WebServer::operator=(const WebServer& toAssign)
{
	(void)toAssign;
	return *this;
}

