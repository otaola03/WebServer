#include "Location.hpp"

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

/* Location::Location(const Location& toCopy) */
/* { */
/* 	(void)toCopy; */
/* } */

Location::~Location()
{
}

Location& Location::operator=(const Location& toAssign)
{
	(void)toAssign;
	return *this;
}

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
