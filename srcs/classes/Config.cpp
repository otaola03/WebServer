/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xmatute- <xmatute-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 10:32:09 by xmatute-          #+#    #+#             */
/*   Updated: 2023/11/05 18:27:49 by xmatute-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

int initServerNum(const std::string &filePath) {
    std::ifstream inputFile(filePath.c_str()); // Abre el archivo usando una cadena de caracteres C
    if (!inputFile) {
        std::cerr << "No se pudo abrir el archivo: " << filePath << std::endl;
        return -1; // Indicar error
    }

    std::string line;
    int count = 0;
    const std::string targetPrefix = "    - server_name:";

    while (std::getline(inputFile, line)) {
        if (line.find(targetPrefix) == 0) {
            count++;
        }
    }

    inputFile.close();
    return count;
}

Config::~Config()
{
	cout << " entrando a Config::~Config() " << endl;
}

Config::Config(const std::string &path) : ifstream(path), path(path), line(), ServerNum(initServerNum(path)), lineNum(0)
{
	cout << " entrando a Config::Config(const std::string &path) path: " << '"' << path << '"' << endl;
	init();
}

string		&Config::nextline()
{
	std::getline(*this, line);
	lineNum++;
	return (line);
}

string		&Config::skipLine(const std::string &expected)
{
	if (nextline() != expected)
		lineException(path + " tiene la linea \"" + line + "\" en vez de \"" + expected + "\".");
	return (line);
}

string	Config::getToken(const std::string &pre)
{
	if (nextline().find(pre) != 0)
		lineException("💮\"" + pre + "\" expected");
	return(line.substr(strlen(pre.c_str())));
}

void	Config::init()
{
	if (!is_open())
		throw std::runtime_error("No se pudo abrir el archivo: " + path);

	skipLine("server:");
	int i = ServerNum;
	while (i--)
	{
	server_name.push_back(getToken(		"    - server_name: "));
	root.push_back(getToken(			"      root: "));
	ports.push_back(parsePorts(getToken("      listen: ").c_str()));
	
	error_pages.push_back(parseErrorPages());

	locations.push_back(parseLocations());
	}
}

intVector	Config::parsePorts(const char *s)
{
	if (*s != '[')
		lineException("'[' expected");
	s++;
	
	intVector	ports;
	
	while (isdigit(*s))
	{
		while (isspace(*s))
			s++;
		if (!isdigit(*s))
			lineException("expected a digit");
		ports.push_back(atoi(s));
		while (isdigit(*s))
			s++;
		while (isspace(*s))
			s++;
		if (*s == ']' || !*s)
			break;
		s++;
		while (isspace(*s))
			s++;
	}
	cout << s << endl;
	if (*s != ']')
		lineException("']' expected");
	return(ports);
}

intCharMap	Config::parseErrorPages()
{
	intCharMap	map;

	skipLine("      error_pages:");
	while (nextline().find("          - ") == 0)
	{
		if (!isdigit(line.substr(line.find("-") + 2)[0]))
			lineException("digit expected");
		if (!line.find(": "))
			lineException("': ' expected");
		map[atoi(line.substr(line.find("- ") + 2).c_str())] = line.substr(line.find(": ") + 2);
	}
	return (map);
}

locationVector	Config::parseLocations()
{
	locationVector	vector;

	skipLine("      locations:");
	while (nextline().find("          - ") == 0)
	{
		Location location(\
				getToken("            path: "),
				getToken("            root: "),
				getToken("            index: "),
				getToken("            allowed_methods: "),
				getToken("            autoindex: "),
				getToken("            max_body_size: "),
				getToken("            redirection: "),
				getToken("            destination: "),
				getToken("            cgi_destinaation: ")
				);
		vector.push_back(location);
		skipLine("");
	}
	return (vector);
}



void	Config::lineException(const std::string &problem)
{
	cerr << "❌" << path << " Parse error at line " << lineNum << ":\n\"" + line + "\"" << endl;
	throw (std::runtime_error(problem));
}

size_t Config::getServerNum() const
{
	return(ServerNum);
}

string			Config::getName(size_t index) const
{
	return(server_name[index]);
}
string			Config::getRoot(size_t index) const
{
	return(root[index]);
}

intVector		Config::getPorts(size_t index) const
{
	return(ports[index]);
}

intCharMap		Config::getErrorPages(size_t index) const
{
	return(error_pages[index]);
}

locationVector	Config::getLocations(size_t index) const
{
	return(locations[index]);
}
