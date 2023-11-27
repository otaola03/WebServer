 #include "WebServer.hpp"
 #include <sys/types.h>
 #include <sys/event.h>
#include "HttpRequest.hpp"

bool	WebServer::isWebServerActivated = true;

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

WebServer::WebServer(const Config& config)
{
    size_t  i = 0;

    while (i < config.getServerNum())
    {
        serversList.push_back(new Server(config.getName(i), config.getRoot(i), config.getPorts(i), config.getErrorPages(i), config.getLocations(i)));
        serversList[i]->addPortsToPortsList(ports);
        /* serversList[i]->addPortsToKq(kq); */

		intPortMap& serverPorts = serversList[i]->getPortsList();
		for (intPortMap::iterator it = serverPorts.begin(); it != serverPorts.end(); ++it)
		{
			kq.addPort(it->first);
			ports[it->first] = it->second;
		}
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
	for (int i = 0; i < (int)serversList.size(); i++)
		delete serversList[i];
}

bool	WebServer::isAPort(int fd)
{
	intPortMap::iterator it = ports.find(fd);
	return (it != ports.end());
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
	/* Server* server; */

    while (isWebServerActivated) 
	{
		numEvents = kq.listenNewEvents();
		for (int i = 0; i < numEvents; i++)
		{
			fd = kq.getEvSet(i).ident;

			// DISCONNECT
			if (kq.getEvSet(i).flags & EV_EOF)
				close(fd);

			// NEW CLIENT
			else if (isAPort(fd))
			{
				int portFd = fd;
				if ((fd = ports[fd]->acceptConnection()) == -1)
					close(fd);
				else
					if (!kq.manageNewConnection(fd))
						close(fd);
				clientsServers[fd] = getServerFromPort(portFd);
			}

			// RECIVE DATA ------- MIRA LO DEL MAXBODY SIZE
			else if (kq.getEvSet(i).filter == EVFILT_READ)
			{
				/* data += recvData(fd); */
				/* clientsData[fd] = recvData(fd); */
				clientsRequests[fd] = new HttpRequest(fd, clientsServers[fd]->getMaxBodySize(), clientsServers[fd]->getLocations());

				/* if (data == "") */
				/* if (clientsData[fd] == "") */
				if (clientsRequests[fd]->getType() == UNDEFINED)
					close(fd);
				else if (clientsRequests[fd]->isUnfinishedRequest())
					kq.disableRead(fd);
				if(!kq.enableWrite(fd))
					close(fd);
				std::cout << clientsRequests[fd]->getType() << "\n";
			}

			// SEND
			else if (kq.getEvSet(i).filter == EVFILT_WRITE)
			{
				/* HttpRequest parser(clientsData[fd]); */

				/* HttpResponse response(parser); */
				HttpResponse response(*clientsRequests[fd]);
				std::string msg = response.getMsg();
				std::cerr << "MSG: " << msg << "\n";

				sendData(fd, msg);

				/* if (send(fd, msg.c_str(), msg.length(), 0) == -1) */
				/* 	perror("send"); */
				kq.manageEndedConnection(fd);
				close(fd);
				delete clientsRequests[fd];
			}
		}
	}
	system("leaks webserver");
}

void	WebServer::signalHandler(int signal)
{
	if (signal == SIGINT || signal == SIGTERM)
		isWebServerActivated = false;
}


WebServer& WebServer::operator=(const WebServer& toAssign)
{
	(void)toAssign;
	return *this;
}
