#include "Location.hpp"

bool isDirectory(const std::string& path)
{
    struct stat info = {};
    if (stat(path.c_str(), &info) != 0)
        return false;
    return S_ISDIR(info.st_mode);
}

Location::Location() : GET(false), POST(false), DELETE(false), autoindex(false)
{
	if (GET && POST && DELETE && autoindex)
		std::cout << "HOLA\n";
}

// Location::Location(
// 	const std::string	path, \
// 	const std::string	root, \
// 	const bool			GET, \
// 	const bool			POST, \
// 	const bool			DELETE, \
// 	const bool			autoindex, \
// 	std::string			redirection \
// ) : path(path), root(root), GET(GET), POST(POST), DELETE(DELETE), autoindex(autoindex)
// {
// 	if (GET && POST && DELETE && autoindex)
// 		std::cout << "HOLA\n";
// }

bool	isOn(const string &on)
{
	if (!(on.find("on") == 0 || on.find("off") == 0))
		throw (std::runtime_error("A location have autoindex \"" + on + "\" (it must be \"on\" or \"off\")"));
	return (on.find("on") == 0);

}

Location::Location(\
			const std::string	&path, \
			const std::string	&root, \
			const std::string	&index, \
			const std::string	&allowed_methods, \
			const std::string	&autoindex, \
			const std::string	&redirection, \
			const std::string	&destination \
) :
path(path),
root(root),
index(index),
allowed_methods(allowed_methods),
GET(isAllowed("GET")),
POST(isAllowed("POST")),
DELETE(isAllowed("DELETE")),
autoindex(isOn(autoindex)),
redirection(redirection),
destination(destination)
{
	check();
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
	if (destination.empty())
		throw (std::runtime_error("Location has an empty destination"));
	size_t i = 0;
	while (allowed_methods[i])
	{
		if (allowed_methods[i] == '[')
			i++;
		while (isspace(allowed_methods[i]))
			i++;
		if (allowed_methods.find("GET", i) == i)
			i += 3;
		else if (allowed_methods.find("POST", i) == i)
			i += 4;
		else if (allowed_methods.find("DELETE", i) == i)
			i += 5;
		else
			throw (std::runtime_error("Location has an invalid allowed_method \"" + allowed_methods + "\""));
		i++;
		while (isspace(allowed_methods[i]))
			i++;
		if (allowed_methods[i] == ']')
			break;
	}


	
	
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
