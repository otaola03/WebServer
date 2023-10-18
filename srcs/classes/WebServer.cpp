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

bool WebServer::fileFinder(const std::string& path)
{
	DIR *dir;
	struct dirent *entry;
	dir = opendir("./resources/images");
	if (dir) {
		while ((entry = readdir(dir)) != NULL) {
			if (entry->d_name == path){
				closedir(dir);
				return (true);
			}
		}
		closedir(dir);
	}
	dir = opendir("./resources/html");
	if (dir) {
		while ((entry = readdir(dir)) != NULL) {
			if (entry->d_name == path){
				closedir(dir);
				return (true);
			}
		}
		closedir(dir);
	}
	return (false);
}

std::string WebServer::getImg(std::string path)
{
	std::string msg = "HTTP/1.1 200 OK";
	msg.append("\nContent-Type: image/png");
	msg.append("\nContent-Length: ");
	std::string html_name = "./resources/images/" + path;
	std::ifstream file(html_name.c_str());
	if (file.is_open()) {
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		msg.append(std::to_string(content.length()));
		msg.append("\n\n");
		msg.append(content);
		file.close();
	} else {
		std::cerr << "Fatal" << std::endl;
	}
	return msg;
}

std::string WebServer::getIndex(std::string path){
	std::string msg = "HTTP/1.1 200 OK";
	msg.append("\nContent-Type: text/html");
	msg.append("\nContent-Length: ");
	std::string html_name = "./resources/html/" + path;
	std::ifstream file(html_name.c_str());
	if (file.is_open()) {
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		msg.append(std::to_string(content.length()));
		msg.append("\n\n");
		msg.append(content);
		file.close();
	} else {
		std::cerr << "Fatal" << std::endl;
	}
	return msg;
}

std::string WebServer::get404()
{
	std::string msg = "HTTP/1.1 404 Not Found";
	msg.append("\nContent-Type: text/html");
	msg.append("\nContent-Length: ");
	std::string html_name = "./resources/html/404.html";
	std::ifstream file(html_name.c_str());
	if (file.is_open()) {
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		msg.append(std::to_string(content.length()));
		msg.append("\n\n");
		msg.append(content);
		file.close();
	} else {
		std::cerr << "Fatal" << std::endl;
	}
	return msg;
}

std::string WebServer::getMessage(HttpRequest& parser)
{
	if (parser.getType() == GET){
		if (parser.getPath() == "/" == true)
			return (getIndex("index.html"));
		else if (fileFinder(parser.getPath().substr(1)) && parser.getPath().find(".html") != std::string::npos)
			return (getIndex(parser.getPath()));
		else if (fileFinder(parser.getPath().substr(1)) && parser.getPath().find(".png") != std::string::npos)
			return (getImg(parser.getPath()));
		else
			return (get404());
	}
	return "";
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
					std::cout << "------> RCV\n";
					if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0)
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
						buf[nbytes] = '\0';
						std::cout << buf << "\n";
						HttpRequest parser(buf);
						std::string mesj = getMessage(parser);
						if (send(i, mesj.c_str(), mesj.size(), 0) == -1)
						{
							perror("send");
						}
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

