#include "WebServer.hpp"

WebServer::WebServer()
{
	intVector	ports;
	ports.push_back(85);
	ports.push_back(105);

	intCharMap	errorPages;
	errorPages[404] = "/404.html";

	Location	location;
	locationVector	locations;
	locations.push_back(location);

	Server *server = new Server("server1", "/", ports, errorPages, locations);
	std::cout << "fdMax: " << server->fdMax << "\n";

	FD_ZERO(&socketList);
	FD_ZERO(&portsList);

	server->addPortsToSet(portsList);

	socketList = portsList;
	serversList.push_back(server);
}

WebServer::WebServer(const WebServer& toCopy)
{
	(void)toCopy;
}

WebServer::~WebServer()
{
}

static void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

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

void	WebServer::serverLoop()
{
	fd_set	read_fds;
	char remoteIP[INET6_ADDRSTRLEN];
	int newfd;
	struct sockaddr_storage remoteaddr;
	socklen_t addrlen;
	int fdmax = serversList[0]->fdMax;
	//char msg[] = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 21\n\n<h1>Hello world!</h1>";
	// std::string mesj = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 21\n\n<h1>Hello world!</h1>";
	std::cout << "----> " << fdmax << "\n";
	// main loop
	while (1) 
	{
		read_fds = socketList; // copy it
		if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
			perror("select");
			exit(4);
		}

		for(int i = 0; i <= fdmax; i++)
		{
			if (FD_ISSET(i, &read_fds)) //Get a connection
			{
				if (FD_ISSET(i, &portsList)) //New Connection
				{
					addrlen = sizeof remoteaddr;
					newfd = accept(i, (struct sockaddr *)&remoteaddr, &addrlen);

					if (newfd == -1)
						perror("accept");
					else
					{
						FD_SET(newfd, &socketList); // add to socketList set
						if (newfd > fdmax)	// keep track of the max
							fdmax = newfd;
						printf("selectserver: new connection from %s on "
							"socket %d\n",
							inet_ntop(remoteaddr.ss_family,
								get_in_addr((struct sockaddr*)&remoteaddr),
								remoteIP, INET6_ADDRSTRLEN),
							newfd);
					}
				}
				else // Connection fron an actual client
				{
					/* std::string data = dynamic_cast<Client*>(connectionsList[i])->recvData(); */
					std::string data = recvData(i);
					/* std::string data; */
					if (data.empty())
					{
                        /* close(i); // bye! */
						/* dynamic_cast<Client*>(connectionsList[i])->closeSockFd(); */
						close(i);
                        FD_CLR(i, &socketList); // remove from socketList set
                    }

					else // Data recived
					{
						/* std::cout << data << "\n"; */
						/* HttpRequest request(data); */
						/* request.printRequest(); */
                        HttpRequest parser(data.c_str());
                        std::string msg = serversList[0]->getMessage(parser);
						// std::cout << "msg: " << msg << "\n";
						if (send(i, msg.c_str(), msg.length(), 0) == -1)
							perror("send");
						close(i);
                        FD_CLR(i, &socketList); // remove from socketList set
                    }
				} // END handle data from client
			} // END got new incoming connection
		} // END looping through file descriptors
	} // END for(;;)--and you thought it would never end!
	std::cout << "------------------------------------------------\n";
}

Server&	WebServer::getServer(const int fd)
{
	(void)fd;
	/* for (serverVector::iterator it = seversList.begin(); it != serversList.end(); ++it) */
	/* { */

	/* } */
	return (*serversList[0]);
}

WebServer& WebServer::operator=(const WebServer& toAssign)
{
	(void)toAssign;
	return *this;
}

