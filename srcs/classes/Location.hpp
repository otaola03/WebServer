#ifndef LOCATION_HPP

# define LOCATION_HPP

#include <iostream>

class Location
{
	private:
		std::string path;
		std::string	root;
		const bool		GET;
		const bool		POST;
		const bool		DELETE;
		const bool		autoindex;
		std::string	redirection;
		std::string	destination;

		Location();

	public:
		Location(\
				const std::string	path, \
				const std::string	root, \
				const bool			GET, \
				const bool			POST, \
				const bool			DELETE, \
				const bool			autoindex, \
				std::string			redirection, \
				std::string			destination \
		);
		/* Location(const Location& toCopy); */
		~Location();

		Location& operator=(const Location& toCopy);
};

#endif
