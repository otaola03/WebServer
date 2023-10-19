#include "WebServer.hpp"
#include "HttpRequest.hpp"

Server*	WebServer::getServerFromPort(int portFd)
{
	for (serverVector::iterator it = serversList.begin(); it != serversList.end(); ++it)
		if ((*it)->containsThisPort(portFd))
			return (std::cout << "LOCOOOOO\n", *it);
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
	server->addPortsToConnectionsList(connectionsList);

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

void	WebServer::serverLoop()
{
	fd_set	read_fds;
	/* char remoteIP[INET6_ADDRSTRLEN]; */
	char buf[100000];
	int newfd;
	/* struct sockaddr_storage remoteaddr; */
	int nbytes;
	/* socklen_t addrlen; */
	int fdmax = serversList[0]->fdMax;
	char msg[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";


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
					newfd = dynamic_cast<Port*>(connectionsList[i])->acceptConnection();

                    if (newfd == -1)
						continue;
                    if (newfd > fdmax)    // keep track of the max
                        fdmax = newfd;
                    FD_SET(newfd, &socketList); // add to socketList set

					Client* newClient = new Client(newfd);	//HAz estas tres cosas en una unica funcion
					getServerFromPort(i)->addClient(newfd, newClient);
					connectionsList[newfd] = newClient;

                }
				
				else // Connection fron an actual client
				{
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) // Hazlo en un bucle
					{
                        // got error or connection closed by client
                        if (nbytes == 0)
                            printf("selectserver: socket %d hung up\n", i);
						else
                            perror("recv");
                        close(i); // bye!
                        FD_CLR(i, &socketList); // remove from socketList set
                    }

					else // Data recived
					{
						// Here i must to know to which server the client belongs
						buf[nbytes] = '\0';
						std::string str(buf);
						HttpRequest request(str);
						/* std::cout << buf << "\n"; */

						request.printRequest();

						if (send(i, msg, sizeof(msg), 0) == -1)
							perror("send");
                    }
                }
            }
    	}
	}
}



WebServer& WebServer::operator=(const WebServer& toAssign)
{
	(void)toAssign;
	return *this;
}

