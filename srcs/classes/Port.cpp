#include "Port.hpp"

static int	getListenFd(struct addrinfo *portInfo)
{
	struct addrinfo	*p;
	int				sockfd;
	
	for (p = portInfo; p; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			perror("port: socket");
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
			perror("port: bind");
			continue ;
		}

		break ;
	}

	if (p == NULL)
	{
		fprintf(stderr, "port: failed to bind\n");
		exit(-1);
	}

	return sockfd;
}

Port::Port() {}

//Pon el constructo vacio privado y haz uno que requeora un puerto como int
Port::Port(const int port) : port(port)
{
	struct addrinfo	templateAddr, *portInfo;
	std::stringstream ss;

	memset(&templateAddr, 0, sizeof templateAddr);
	templateAddr.ai_flags = AI_PASSIVE;
	templateAddr.ai_family = AF_UNSPEC;
	templateAddr.ai_socktype = SOCK_STREAM;

	int status;

	// std::string stringPort = std::to_string(port);
	ss << port;
	std::string stringPort = ss.str();
	if ((status = getaddrinfo(NULL, stringPort.c_str(), &templateAddr, &portInfo)) != 0)
	{
    	fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    	exit(1);
	}

	sockfd = getListenFd(portInfo);
	freeaddrinfo(portInfo);
}

Port::Port(const Port& toCopy)
{
	(void)toCopy;
}

Port::~Port()
{
	std::cout << "Port closed\n";
	close(sockfd);
}

int	Port::getSockFd() {return sockfd;}

void	Port::activatePort()
{
	if (listen(sockfd, BACKLOG) == -1)
	{
		perror("listen");
		exit(-1);
	}
}

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
	std::cout << "port: got conection from " << s << "\n";
}

int	Port::acceptConnection()
{
	int new_fd;
	socklen_t sin_size;
	struct sockaddr_storage their_addr;

   sin_size = sizeof their_addr;
   new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
   if (new_fd == -1)
       perror("accept");
   if (new_fd != -1)
	   printClientInfo(their_addr);
   return (new_fd);
}

void	Port::closePort() {close(sockfd);}

Port& Port::operator=(const Port& toAssign)
{
	std::cout << "ASGINADO\n"; 
	sockfd = toAssign.sockfd;
	port = toAssign.port;
	return *this;
}

