#include "Config.hpp"

int Config::firstCheck(const std::string &filePath) //parche guarro
{
    std::ifstream inputFile(filePath.c_str());
    if (!inputFile) {
        std::cerr << "No se pudo abrir el archivo: " << filePath << std::endl;
        return -1;
    }

    std::string line;
    int count = 0;
    std::string targetPrefix = "    - server_name:";

	std::getline(inputFile, line);
	std::getline(inputFile, line);	
    if (line.find(targetPrefix) != 0)
	{
		this->line = line;
		lineNum = 2;
		lineException(filePath + " tiene la linea \"" + line + "\" en vez de \"" + targetPrefix + "\".");
	}

	while (std::getline(inputFile, line)) {		
		if (line.find("        - ") == 0) {
			targetPrefix = "            path: ";
			std::getline(inputFile, line);
			if (line.find(targetPrefix) != 0)
			{
				this->line = line;
				lineNum = 2;
				lineException(filePath + " tiene la linea \"" + line + "\" en vez de \"" + targetPrefix + "\".");
			}
			targetPrefix = "          - ";
		}
}
    inputFile.close();
    return count;
}

int Config::initServerNum(const std::string &filePath) {
	firstCheck(filePath);
    std::ifstream inputFile(filePath.c_str());
    if (!inputFile) {
        std::cerr << "No se pudo abrir el archivo: " << filePath << std::endl;
        return -1;
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
	this->close();
}

Config::Config(const std::string &path) : ifstream(path), path(path), line(), ServerNum(initServerNum(path)), lineNum(0)
{
	init();
	check();
}

bool	haveRoot(locationVector	locations)
{
	for (locationVector::const_iterator i = locations.begin(); i != locations.end(); i++)
	{
		if (i->getPath() == "/")
			return true;
	}
	return false;
}

void	Config::check()
{
	for (std::vector<string>::const_iterator i = server_name.begin(); i != server_name.end(); i++)
		if (i->empty())
			throw (std::runtime_error("server_name can not be empty"));
	
	for (std::vector<size_t>::const_iterator i = max_body_size.begin(); i != max_body_size.end(); i++)
		if (*i == 0)
			throw (std::runtime_error("invalid max_body_size"));

	for (std::vector<locationVector>::const_iterator i = locations.begin(); i != locations.end(); i++)
		if (!haveRoot(*i))
			throw (std::runtime_error("Root location (path: /) missing"));
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
		lineException("\"" + pre + "\" expected");
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
	ports.push_back(parsePorts(getToken("      listen: ").c_str()));
	max_body_size.push_back(atol(getToken("      max_body_size: ").c_str()));
	
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
	Location		temp;
	int 			i = 0;

	skipLine("      locations:");
	while (nextline().find("          - ") == 0)
	{
		Location location(\
				getToken("            path: "),
				getToken("            root: "),
				getToken("            index: "),
				getToken("            allowed_methods: "),
				getToken("            autoindex: "),
				getToken("            redirection: "),
				getToken("            destination: "),
				getToken("            cgi-bin: ")
				);
		if (location.getPath() == "/"){
			temp = location;
			i = 1;
			skipLine("");
			continue;}
		vector.push_back(location);
		skipLine("");
	}
	if (i == 1)
		vector.push_back(temp);
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

size_t			Config::getMaxBodySize(size_t index) const
{
	return(max_body_size[index]);
}