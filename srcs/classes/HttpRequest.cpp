#include "HttpRequest.hpp"

// HttpRequest::HttpRequest()
// {
// }

static	RequestType whatTypeIs(const std::string& type)
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

/* void	HttpRequest::saveBody(const char* toProcess) */
/* { */
/* 	std::string contentLength; */
/* 	contentLength = std::strstr(toProcess, "content-length: ") ? std::strstr(toProcess, "content-length: ") : std::strstr(toProcess, "Content-Length: "); */
/* 	if (contentLength.empty()) */
/* 		return ; */
/* 	int length = std::atoi(contentLength.c_str() + 16); */
/* 	const char* body_t = std::strstr(toProcess, "\r\n\r\n"); */
/* 	body = std::string(body_t, length); */
/* 	body = body.substr(body.find("\r\n\r\n") + 4); */
/* 	size_t lastLine = body.find_last_of("\n"); */
/* 	body = body.substr(0, lastLine); */
/* } */

void	HttpRequest::saveBody(const std::string& toProcess)
{
	/* std::cout << toProcess << "\n"; */
	/* std::string auxBody = toProcess.substr(toProcess.find("\r\n\r\n") + 4); */
	/* std::cout << BLUE << "====================================\n" << WHITE; */
	/* std::cout << auxBody << "\n"; */
	/* std::cout << BLUE << "====================================\n" << WHITE; */

	/* body = auxBody.substr(auxBody.find("\r\n\r\n")); */
	/* std::cout << body.find("\r\n\r\n") << "\n"; */
	body = toProcess.substr(toProcess.find("\r\n\r\n") + 4);
	std::cout << RED << "====================================\n" << WHITE;
	std::cout << toProcess << "\n";
	std::cout << RED << "====================================\n" << WHITE;
}

void	HttpRequest::printBody()
{
	if (body.empty()){
		std::cerr << "No body to print\n";
		return ;
	}
	for (std::string::iterator it = body.begin(); it != body.end(); ++it){
		if (*it == '\n')
			std::cout << "\\n";
		else if (*it == '\r')
			std::cout << "\\r";
		else
			std::cout << *it;
	}
}

std::string	HttpRequest::getBody()
{
	return body;
}

void	HttpRequest::printHeaders()
{
	for (std::map<std::string, std::string>::iterator it = headers.begin(); \
		it != headers.end(); ++it)
		std::cout << (*it).first << ": " << (*it).second << "\n";
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
		// std::pair<std::string, std::string> auxPair = std::make_pair(\
		// 	it->substr(0, p), it->substr(p + 2));
		headers[it->substr(0, p)] = it->substr(p + 2);
		// headers.push_back(auxPair);
	}
}


HttpRequest::HttpRequest(const std::string& toProcess)
{
	saveRequest(toProcess);
	saveHeaders(toProcess);
	if (type == POST)
		saveBody(toProcess);
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
	for (std::map<std::string, std::string>::iterator it = headers.begin(); \
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
