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

WebServer::WebServer(const Config& config)
{
    size_t  i = 0;

	kq = kqueue();
    while (i < config.getServerNum())
    {
        serversList.push_back(new Server(config.getName(i), config.getRoot(i), config.getPorts(i), config.getErrorPages(i), config.getLocations(i)));
        serversList[i]->addPortsToPortsList(ports);
        serversList[i]->addPortsToKq(kq);
		i++;
    }
	// socketList = portsList;
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
	std::cerr << "🌎es aqui babygirl?\n";
	intPortMap::iterator it = ports.find(fd);
	std::cerr << "🍪dimelo tu\n";
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

void	WebServer::serverLoop()
{
	//char msg[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\nConnection close\r\n\r\nHello world!";
	struct kevent evList[MAX_EVENTS];
	int numEvents;
	int	fd;
	std::string data;

    while (1) 
	{
		std::cerr << "\nwhile (1)\n";
		memset(evList, 0, sizeof(evList));
		numEvents = kevent(kq, NULL, 0, evList, 1, NULL);
		std::cerr << "kq: " << kq << "\n";
		std::cerr << "numEvents: " << numEvents << "\n";
		for (int i = 0; i < numEvents; i++)
		{
			std::cerr << "evento: " << i << "\n";
			fd = evList[i].ident;
			std::cerr << "fd: " << fd << "\n";

			// DISCONNECT
			if (evList[i].flags & EV_EOF){
				std::cerr << "DISCONNECT\n";
				deleteClient(fd);
			}
			

			// NEW CLIENT
			else if (isAPort(fd))
			{
				std::cerr << "🍱por aqui pasamos\n";
				if (!acceptNewClient(fd))
				{
					std::cerr << "acceptNewClient error\n";
					close(fd);
					delete clients[fd];
				}
				std::cerr << "aceptamos cliente\n";
			}
			// RECIVE DATA
			else if (evList[i].filter == EVFILT_READ)
			{
				std::cerr << "RECIVE DATA\n";

				data = clients[fd]->recvData();
				std::ofstream file("FOCAAA");
				file << data << std::endl;
				if (data == "")
					deleteClient(fd);
				else
					clients[fd]->enable_event(kq, EVFILT_WRITE);
			}

			// SEND
			else if (evList[i].filter == EVFILT_WRITE)
			{
				std::cerr << "SEND\n";
				std::cerr << "data: " << data << "\n";


				HttpRequest parser(data.c_str());
				std::cerr << "parser instanciado\n";

				std::string msg = getServerFromClient(fd)->getMessage(parser);
				// std::cerr << "msg: " << msg << "\n";
				if (send(fd, msg.c_str(), msg.length(), 0) == -1)
					perror("send");
				clients[fd]->disable_event(kq, EVFILT_WRITE);
				deleteClient(fd);
			}
		}
	}
}



WebServer& WebServer::operator=(const WebServer& toAssign)
{
	(void)toAssign;
	return *this;
}
