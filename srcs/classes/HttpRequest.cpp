#include "HttpRequest.hpp"

HttpRequest::HttpRequest()
{
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

void	HttpRequest::saveRequest(const std::string& toProcess)
{
	std::string request = toProcess.substr(0, toProcess.find('\n'));

	size_t p = request.find(' ');
	type = whatTypeIs(request.substr(0, p));

	size_t p2 = request.find(' ', p + 1);
	path = request.substr(p + 1, p2 - p);
	path = path.erase(path.size() - 1);
}


static void	splitRequest(std::vector<std::string>& lines, const std::string& toProcess)
{
	std::istringstream stream(toProcess);
	std::string	line;

	while (std::getline(stream, line))
		if (!line.empty())
			lines.push_back(line);
}

void	HttpRequest::saveHeaders(const std::string& toProccess)
{
	std::vector<std::string> lines;
	splitRequest(lines, toProccess);


	for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it)
	{
		size_t p = it->find(':');
		if (p == std::string::npos)
			continue;
		std::pair<std::string, std::string> auxPair = std::make_pair(\
			it->substr(0, p), it->substr(p + 2));
		headers.push_back(auxPair);
	}
}


HttpRequest::HttpRequest(const std::string& toProcess)
{
	saveRequest(toProcess);
	saveHeaders(toProcess);
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
	for (std::vector< std::pair<std::string, std::string> >::iterator it = headers.begin(); \
		it != headers.end(); ++it)
		std::cout << (*it).first << ": " << (*it).second << "\n";
}

HttpRequest& HttpRequest::operator=(const HttpRequest& toAssign)
{
	(void)toAssign;
	return *this;
}

int		HttpRequest::getType()
{
	return type;
}

std::string	HttpRequest::getPath()
{
	return path;
}