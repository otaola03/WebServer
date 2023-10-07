#include "Server.hpp"

static int	getListenFd(struct addrinfo *serverInfo)
{
	struct addrinfo	*p;
	int				sockfd;
	
	for (p = serverInfo; p; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			perror("server: socket");
			continue ;
		}

		int yes = 1;
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		{
			perror("setsockopt");
			exit(1);
		}
		
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			perror("server: bind");
			continue ;
		}

		break ;
	}

	if (p == NULL)
	{
		fprintf(stderr, "server: failed to bind\n");
		exit(-1);
	}

	return sockfd;
}

//Igual haz una funcion para construir el servidor y otra para hacer el listen
void	Server::startServer(int port)
{
	struct addrinfo	templateAddr, *serverInfo;

	memset(&templateAddr, 0, sizeof templateAddr);
	templateAddr.ai_flags = AI_PASSIVE;
	templateAddr.ai_family = AF_UNSPEC;
	templateAddr.ai_socktype = SOCK_STREAM;

	int status;

	std::string stringPort = std::to_string(port);
	if ((status = getaddrinfo(NULL, stringPort.c_str(), &templateAddr, &serverInfo)) != 0)
	{
    	fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    	exit(1);
	}

	listenSocket = getListenFd(serverInfo);
	freeaddrinfo(serverInfo);

	if (listen(listenSocket, BACKLOG) == -1)
	{
		perror("listen");
		exit(-1);
	}
}

//Pon el constructo vacio privado y haz uno que requeora un puerto como int
Server::Server()
{
	startServer(80);
	(void)master;
}

Server::Server(const Server& toCopy)
{
	(void)toCopy;
}

Server::~Server()
{
	std::cout << "Server closed\n";
	close(listenSocket);
}

int	Server::getListenSocket() {return listenSocket;}

static void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)sa)->sin_addr);
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

static void	printClientInfo(struct sockaddr_storage &their_addr)
{
	char s[INET6_ADDRSTRLEN];
	inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
	std::cout << "server: got conection from " << s << "\n";
}

int	Server::acceptConnection()
{
	int new_fd;
	socklen_t sin_size;
	struct sockaddr_storage their_addr;

   sin_size = sizeof their_addr;
   new_fd = accept(listenSocket, (struct sockaddr *)&their_addr, &sin_size);
   if (new_fd == -1)
       perror("accept");
   if (new_fd != -1)
	   printClientInfo(their_addr);
   return (new_fd);
}

void	Server::closeServer() {close(listenSocket);}

Server& Server::operator=(const Server& toAssign)
{
	(void)toAssign;
	return *this;
}

