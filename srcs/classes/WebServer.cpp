#include "WebServer.hpp"

// WebServer::WebServer()
// {
// 	intVector	ports;
// 	ports.push_back(85);
// 	ports.push_back(105);

// 	intCharMap	errorPages;
// 	errorPages[404] = "/404.html";

// 	Location	location;
// 	locationVector	locations;
// 	locations.push_back(location);

// 	Server *server = new Server("server1", "/", ports, errorPages, locations);
// 	std::cout << "fdMax: " << server->fdMax << "\n";

// 	FD_ZERO(&socketList);
// 	FD_ZERO(&portsList);

// 	server->addPortsToSet(portsList);

// 	socketList = portsList;
// 	serversList.push_back(server);
// }

WebServer::WebServer(const Config& config)
{
    size_t  i = 0;

    // FD_ZERO(&socketList);
	// FD_ZERO(&portsList);
    while (i < config.getServerNum())
    {
        serversList.push_back(new Server(config.getName(i), config.getRoot(i), config.getPorts(i), config.getErrorPages(i), config.getLocations(i)));
        // serversList[i]->addPortsToSet(portsList);
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
	char buf[256];
	int newfd;
	struct sockaddr_storage remoteaddr;
	int nbytes;
	socklen_t addrlen;
	int fdmax = serversList[0]->fdMax;

	std::cout << "----> " << fdmax << "\n";
	// main loop
    while (1) 
	{
        read_fds = socketList; // copy it
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }

        // run through the existing connections looking for data to read
        for(int i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) { // we got one!!
                if (FD_ISSET(i, &portsList)) {
                    // handle new connections
                    addrlen = sizeof remoteaddr;
                    newfd = accept(i,
                        (struct sockaddr *)&remoteaddr,
                        &addrlen);

                    if (newfd == -1) {
                        perror("accept");
                    } else {
                        FD_SET(newfd, &socketList); // add to socketList set
                        if (newfd > fdmax) {    // keep track of the max
                            fdmax = newfd;
                        }
                        printf("selectserver: new connection from %s on "
                            "socket %d\n",
                            inet_ntop(remoteaddr.ss_family,
                                get_in_addr((struct sockaddr*)&remoteaddr),
                                remoteIP, INET6_ADDRSTRLEN),
                            newfd);
                    }
                } else {
                    // handle data from a client
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
                        // got error or connection closed by client
                        if (nbytes == 0) {
                            // connection closed
                            printf("selectserver: socket %d hung up\n", i);
                        } else {
                            perror("recv");
                        }
                        close(i); // bye!
                        FD_CLR(i, &socketList); // remove from socketList set
                    } else {
                        // we got some data from a client
                        for(int j = 0; j <= fdmax; j++) {
                            // send to everyone!
                            if (FD_ISSET(j, &socketList)) {
                                // except the listener and ourselves
                                if (!FD_ISSET(j, &portsList) && j != i) {
                                    if (send(j, buf, nbytes, 0) == -1) {
                                        perror("send");
                                    }
                                }
                            }
                        }
                    }
                } // END handle data from client
            } // END got new incoming connection
    	} // END looping through file descriptors
	} // END for(;;)--and you thought it would never end!
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

