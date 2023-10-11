#ifndef SERVER_HPP

# define SERVER_HPP

#include "Port.hpp"
#include <vector>
#include <map>
#include <algorithm>

typedef std::vector<Port> portsVector;
typedef std::map<int, Port> portMap;

class Server
{
	private:
		std::string name;
		portMap	fdPortList;	//map(fd, Port)

	public:
		Server();
		Server(const std::string& name, const std::vector<int> ports);
		Server(const Server& toCopy);
		~Server();

		Port&	getPort(const int fd);

		Server& operator=(const Server& toAssign);
};

#endif
