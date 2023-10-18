#include "WebServer.hpp"
#include "HttpRequest.hpp"

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

void	WebServer::serverLoop()
{
	fd_set	read_fds;
	char remoteIP[INET6_ADDRSTRLEN];
	char buf[100000];
	int newfd;
	struct sockaddr_storage remoteaddr;
	int nbytes;
	socklen_t addrlen;
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
                    addrlen = sizeof remoteaddr;
                    newfd = accept(i, (struct sockaddr *)&remoteaddr, &addrlen);
					// Creat a client object --> client(newfd, *server)

                    if (newfd == -1)
                        perror("accept");
                    else
					{
                        FD_SET(newfd, &socketList); // add to socketList set
                        if (newfd > fdmax)    // keep track of the max
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

// ---------------->>> Haz una clase connection con los metodos send y recv. La clase Port y la clase Client heredan de esta


WebServer& WebServer::operator=(const WebServer& toAssign)
{
	(void)toAssign;
	return *this;
}

