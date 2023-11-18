#include "Server.hpp"

Server::Server()
{
}

Server::Server(\
	const std::string& name, \
	const std::string& root, \
	const intVector& ports, \
	const intCharMap& errorPages, \
	const locationVector& locations\
) : name(name), root(root), errorPages(errorPages), locations(locations)
{
	for (int i = 0; i != (int)ports.size(); i++)
	{
		Port *port = new Port(ports[i]);
		fdMax = port->getSockFd();
		port->activatePort();
		fdPortsList[port->getSockFd()] = port;
	}
}

Server::Server(const Server& toCopy)
{
	(void)toCopy;
}

Server::~Server()
{
}

intPortMap& Server::getPortsList() {return fdPortsList;}


//------------------------------------------------------------------------------------------
/* void	Server::addPortsToSet(fd_set& portsFdSet) */
/* { */
/* 	for (intPortMap::iterator it = fdPortsList.begin(); it != fdPortsList.end(); ++it) */
/* 		FD_SET(it->first, &portsFdSet); */
/* } */

/* void	Server::addPortsToConnectionsList(intConnectionMap& connectionsList) */
/* { */
/* 	for (intPortMap::iterator it = fdPortsList.begin(); it != fdPortsList.end(); ++it) */
/* 		connectionsList[it->first] = it->second; */
/* } */
//------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------
void	Server::addPortsToPortsList(intPortMap& portsList)
{
	for (intPortMap::iterator it = fdPortsList.begin(); it != fdPortsList.end(); ++it)
		portsList[it->first] = it->second;
}
//------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------
/* void	Server::addPortsToKq(int kq) */
/* { */
/* 	for (intPortMap::iterator it = fdPortsList.begin(); it != fdPortsList.end(); ++it) */
/* 	{ */
/* 		std::cout << "i\n"; */
/* 		kevent(kq, &it->second->getEvSet(), 1, NULL, 0, NULL); */
/* 	} */
/* } */
//------------------------------------------------------------------------------------------


void	Server::addClient(int clientFd, Client* client) {fdClientsList[clientFd] = client;}

bool	Server::containsThisPort(int portFd)
{
	intPortMap::iterator it = fdPortsList.find(portFd);
	return (it != fdPortsList.end());
}

bool	Server::containsThisClient(int clientFd)
{
	intClientMap::iterator it = fdClientsList.find(clientFd);
	return (it != fdClientsList.end());
}

Server& Server::operator=(const Server& toAssign)
{
	(void)toAssign;
	return *this;
}

