#include "Location.hpp"

bool isDirectory(const std::string& path)
{
    struct stat info = {};
    if (stat(path.c_str(), &info) != 0)
        return false;
	std::cout << S_ISDIR(info.st_mode) << std::endl;
    return S_ISDIR(info.st_mode);
}

Location::Location() : GET(false), POST(false), DELETE(false), autoindex(false)
{
	if (GET && POST && DELETE && autoindex)
		std::cout << "HOLA\n";
}

Location::Location(
	const std::string	path, \
	const std::string	root, \
	const bool			GET, \
	const bool			POST, \
	const bool			DELETE, \
	const bool			autoindex, \
	std::string			redirection, \
	std::string			destination
) : path(path), root(root), GET(GET), POST(POST), DELETE(DELETE), autoindex(autoindex), redirection(redirection), destination(destination)
{
	if (GET && POST && DELETE && autoindex)
		std::cout << "HOLA\n";
}

Location::Location(\
				const std::string	&path, \
				const std::string	&root, \
				const std::string	&index, \
				const std::string	&allowed_methods, \
				const std::string	&autoindex, \
				const std::string	&redirection, \
				const std::string	&destination, \
				const std::string	&cgi_destinaation \
) :
path(path),
root(root),
index(index),
allowed_methods(allowed_methods),
GET(isAllowed("GET")),
POST(isAllowed("POST")),
DELETE(isAllowed("DELETE")),
autoindex(autoindex.find("on") == 0),///hmmmm
redirection(redirection),
destination(destination),
cgi_destinaation(cgi_destinaation)
{
	check();
	// if (autoindex)
	// 	createAutoindex();
	
}



#include "iostream"
void Location::check()
{
	if (path[0] != '/')
		throw (std::runtime_error("Location's path \"" + path + "\" does not start with '/'"));
	if (!isDirectory(root))//esto no va
		throw (std::runtime_error("Location's root \"" + root + "\" is not a directoy"));
	if (index.empty() && !autoindex)
		throw (std::runtime_error("Location has not index and autoindex is not on"));
	if (!index.empty() && autoindex)
		throw (std::runtime_error("Location has index and autoindex is on"));
	// if (!index.empty() && isDirectory(index))
	// 	throw (std::runtime_error("Location's index is not a directory"));//??
}

/* Location::Location(const Location& toCopy) */
/* { */
/* 	(void)toCopy; */
/* } */

Location::~Location()
{
}

// Location& Location::operator=(const Location& toAssign)
// {
// 	(void)toAssign;
// 	return *this;
// }

std::string	Location::getPath() const {return path;};
bool	Location::isGET() const
{
	return GET;
}

bool	Location::isPOST() const
{
	return POST;
}


bool	Location::isDELETE() const
{
	return DELETE;
}

bool	Location::isAllowed(const std::string	&method)
{
	return(allowed_methods.find(method) != std::string::npos);
}

std::string	Location::getRoot() const
{
	return root;
}
