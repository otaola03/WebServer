#ifndef LOCATION_HPP
# define LOCATION_HPP
#include <iostream>
class Location
{
	private://mejor const
		std::string path;
		std::string	root;
		std::string	index;
		std::string	allowed_methods;
		bool	GET;
		bool	POST;
		bool	DELETE;
		bool	autoindex;
		size_t		max_body_size;
		std::string	redirection;
		std::string	destination;
		std::string	cgi_destinaation;

		// Location();
		bool	isAllowed(const std::string	&method);


	public:
		Location();

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
		Location(\
			const std::string	&path, \
			const std::string	&root, \
			const std::string	&index, \
			const std::string	&allowed_methods, \
			const std::string	&autoindex, \
			const std::string	&max_body_size, \
			const std::string	&redirection, \
			const std::string	&destination, \
			const std::string	&cgi_destinaation \
		);
		/* Location(const Location& toCopy); */
		~Location();

		std::string	getPath() const;

		bool		isGET() const;
		bool		isPOST() const;
		bool		isDELETE() const;
		std::string	getRoot() const;
		//Location& operator=(const Location& toCopy);
};

#endif