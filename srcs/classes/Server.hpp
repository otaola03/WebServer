#ifndef SERVER_HPP

# define SERVER_HPP

#include "Port.hpp"
#include "Location.hpp"

#include "../../includes/templates.h"

#include <algorithm>
#include <exception>


class Server
{
	private:
		std::string name;
		std::string root;
		intPortMap	fdPortsList;	//map(fd, Port)
		intCharMap errorPages;
		locationVector locations;

		Server();

	public:
		int fdMax;

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

		void	addPortsToSet(fd_set& portsList);

		Server& operator=(const Server& toAssign);
};

#endif
