

#ifndef Config_HPP
# define Config_HPP

#include <fstream>
using	std::ifstream;

#include <iostream>
using	std::cerr;
using	std::cout;
using	std::endl;

#include <string>
using	std::string;

#include <vector>
#include <map>

#include <cstdlib>

// #include "templates.h"

typedef std::vector<int> intVector;//ta feo k este aqui
typedef std::map<int, std::string> intCharMap;
#include "Location.hpp"
typedef std::vector<Location> locationVector;



class Config : protected ifstream
{
	private:
		string	path;
		string	line;
		size_t	ServerNum;
		size_t	lineNum;

		std::vector<string>			server_name;
		std::vector<string>			root;
		std::vector<size_t>			max_body_size;
		std::vector<intVector>		ports;
		std::vector<intCharMap>		error_pages;
		std::vector<locationVector>	locations;

		void		init();
		void		check();
		string		&nextline();
		string		&skipLine(const std::string &expected);
		void		lineException(const std::string &problem);
		string		getToken(const std::string &pre);
		intVector	parsePorts(const char *s);
		intCharMap	parseErrorPages();
		locationVector	parseLocations();
		bool		isLocation(const std::string &to_find);
		
	public:
		Config(const std::string &path);
		~Config();

		size_t 			getServerNum() const;
		string			getName(size_t index) const;
		string			getRoot(size_t index) const;
		intVector		getPorts(size_t index) const;
		intCharMap		getErrorPages(size_t index) const;
		locationVector	getLocations(size_t index) const;
};


#endif
