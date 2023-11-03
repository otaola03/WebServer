#ifndef SERVER_HPP

# define SERVER_HPP

#include "Port.hpp"
#include "Location.hpp"
#include "Client.hpp"

#include "../../includes/templates.h"

#include <algorithm>
#include <exception>
typedef std::map<int, Client*> intClientMap;
typedef std::map<int, Port*> intPortMap;


class Server
{
	private:
		std::string 	name;
		std::string 	root;
		intPortMap		fdPortsList;	//map(fd, Port*)
		intClientMap	fdClientsList;	//map(fd, Client*)
		intCharMap		errorPages;
		locationVector	locations;

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

		/* void	addPortsToSet(fd_set& portsList); */
		/* void	addPortsToConnectionsList(intConnectionMap& connectionsList); */
		void	addClient(int clientFd, Client* client);

		bool	containsThisPort(int portFd);
		bool	containsThisClient(int clientFd);

		void	addPortsToPortsList(intPortMap& portsList);
		void	addPortsToClientsList(intClientMap& clientsList);

		void	addPortsToKq(int kq);

		Server& operator=(const Server& toAssign);
};

#endif
