#ifndef SERVER_HPP

# define SERVER_HPP

#include "Port.hpp"
#include "Location.hpp"
#include <vector>
#include <map>
#include <algorithm>

typedef std::vector<int> intVector;
typedef std::vector<Port> portsVector;
typedef std::vector<Location> locationVector;
typedef std::map<int, Port> intPortMap;
typedef std::map<int, std::string> intCharMap;

class Server
{
	private:
		std::string name;
		std::string root;
		intPortMap	fdPortList;	//map(fd, Port)
		intCharMap errorPages;
		locationVector locations;

		Server();

	public:
		Server(\
				const std::string& name, \
				const std::string& root, \
				const intVector& ports, \
				const intCharMap& errorPages, \
				const locationVector& locations\
		);
		Server(const std::string& name, const std::vector<int> ports);
		Server(const Server& toCopy);
		~Server();

		Port&	getPort(const int fd);

		Server& operator=(const Server& toAssign);
};

#endif
