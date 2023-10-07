#include "HttpRequest.hpp"

HttpRequest::HttpRequest()
{
}

static void	splitRequest(std::vector<std::string>& lines, const std::string& toProcess)
{
	std::istringstream stream(toProcess);
	std::string	line;

	while (std::getline(stream, line))
		if (!line.empty())
			lines.push_back(line);
}

static	RequestType whatTypeIs(std::string type)
{
	if (type == "GET")
		return GET;
	else if (type == "POST")
		return POST;
	else if (type == "DELETE")
		return DELETE;
	return UNDEFINED;
}

//Ponlo bonito
HttpRequest::HttpRequest(std::string toProcess)
{
	std::vector<std::string> lines;
	splitRequest(lines, toProcess);


	for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it)
	{
		size_t p = it->find(':');
		if (p == std::string::npos)
			continue;
		std::pair<std::string, std::string> auxPair = std::make_pair(\
			it->substr(0, p), it->substr(p + 2));
		request.push_back(auxPair);
	}


	size_t p = lines[0].find(' ');
	type = whatTypeIs(lines[0].substr(0, p));
	size_t p2 = lines[0].find(' ', p + 1);
	source = lines[0].substr(p + 1, p2 - p);
	std::cout << source << "\n";
}

HttpRequest::HttpRequest(const HttpRequest& toCopy)
{
	(void)toCopy;
}

HttpRequest::~HttpRequest()
{
}

void	HttpRequest::printRequest()
{
	for (std::vector< std::pair<std::string, std::string> >::iterator it = request.begin(); \
		it != request.end(); ++it)
		std::cout << (*it).first << ": " << (*it).second << "\n";
}

HttpRequest& HttpRequest::operator=(const HttpRequest& toAssign)
{
	(void)toAssign;
	return *this;
}

