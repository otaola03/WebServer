#include "Location.hpp"

bool isDirectory(const std::string& path)
{
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
        return false;
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
	if (isDirectory(root))
		throw (std::runtime_error("Location's root \"" + root + "\" is not a directoy"));
	if (index.empty() && !autoindex)
		throw (std::runtime_error("Location has not index and autoindex is not on"));
	if (!index.empty() && autoindex)
		throw (std::runtime_error("Location has index and autoindex is on"));
	if (!index.empty() && isDirectory(index))
		throw (std::runtime_error("Location's index is not a directory"));//??
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

bool	Location::isAllowed(const std::string	&method)
{
	return(allowed_methods.find(method) != std::string::npos);
}

vector<struct dirent*>  getFiles(const string& directory)
{
    DIR* dir = opendir(directory.c_str());
    if (!dir) {
        cerr << "No se puede abrir el directorio " << directory << endl;//iug
    }
    vector<struct dirent*> files;
    while (struct dirent* file = readdir(dir)) {
        files.push_back(file);
    }
    closedir(dir);
    return(files);
}

vector<struct dirent*>  Location::getFiles()
{
	return(::getFiles(root));
}

void	listFiles(const string& directory)
{
  vector<struct dirent*> files = getFiles(directory);

	// cout << endl << "🎱 entramos con " << directory << endl;
	for (size_t i = 0; i < files.size(); i++)
	{
		struct dirent* file = files[i];
		// cout << endl << "🎗 " << i << " " << file->d_name << endl;
		
		if (file->d_name[0] && file->d_name[0] != '.')
		{
			if (!isDirectory(directory + "/" + file->d_name))
			{
				cout << "<li><a href=\"" << directory + "/" + file->d_name << "\">";
				cout << directory + "/" + file->d_name;
				cout << "</a></li>\n";
			}
			else
				listFiles(directory + "/" + file->d_name);
		}
	}
}

void Location::generate_autoindex_http()
{
	ofstream	file("autoindex.html");
	cout << "HTTP/1.1 200 OK\n\n";
	cout << "<html>\n";
	cout << "<head>\n";
	cout << "<title>Autoindex</title>\n";
	cout << "</head>\n";
	cout << "<body>\n";
	cout << "<ul>\n";

	listFiles(root);

	cout << "</ul>\n";
	cout << "</body>\n";
	cout << "</html>\n";
}