#include "Server.hpp"

Server::Server()
{
}

std::map <int, std::string> Server::getErrorPages()
{
	return errorPages;
}

Server::Server(\
	const std::string& name, \
	const std::string& root, \
	const intVector& ports, \
	const intCharMap& errorPages, \
	const locationVector& locations\
) : name(name), root(root), errorPages(errorPages), locations(locations), maxBodySize(5000000)
{
	std::cerr << "construimos " << name << "\n";
	for (int i = 0; i != (int)ports.size(); i++)
	{
		std::cerr << "Creating port " << ports[i] << "\n";
		Port *port = new Port(ports[i]);
		fdMax = port->getSockFd();
		port->activatePort();
		fdPortsList[port->getSockFd()] = port;
	}
	std::cout << "fdMax: " << fdMax << "\n";
}

Server::Server(const Server& toCopy)
{
	(void)toCopy;
}

Server::~Server()
{
	for (intPortMap::iterator it = fdPortsList.begin(); it != fdPortsList.end(); ++it)
		delete it->second;
}

intPortMap& Server::getPortsList() {return fdPortsList;}

int	Server::getMaxBodySize() {return maxBodySize;}


locationVector&	Server::getLocations() {return locations;}


//------------------------------------------------------------------------------------------
void	Server::addPortsToPortsList(intPortMap& portsList)
{
	for (intPortMap::iterator it = fdPortsList.begin(); it != fdPortsList.end(); ++it)
		portsList[it->first] = it->second;
}
//------------------------------------------------------------------------------------------



bool	Server::containsThisPort(int portFd)
{
	intPortMap::iterator it = fdPortsList.find(portFd);
	return (it != fdPortsList.end());
}

Server& Server::operator=(const Server& toAssign)
{
	(void)toAssign;
	return *this;
}

std::ostream& Server::operator<<(std::ostream& os) const
{
	os << "Name: " << name << "\n";
	os << "root: " << root << "\n";
	return os;
}
