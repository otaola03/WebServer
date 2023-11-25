#include <iostream>
using	std::cerr;
using	std::cout;
using	std::endl;

#include <string>
using	std::string;

const string defaultPath = "config/default.yml";

#include "./srcs/classes/Config.hpp"
#include "./srcs/classes/WebServer.hpp"

// void testConfig() {
//     Config config(defaultPath);

//     size_t numServers = config.getServerNum();

//     for (size_t i = 0; i < numServers; i++) {
//         std::cout << "Server " << i << ":\n";
//         std::cout << "Name: " << config.getName(i) << "\n";
//         std::cout << "Root: " << config.getRoot(i) << "\n";
//         std::cout << "Ports: ";
//         intVector ports = config.getPorts(i);
//         for (size_t j = 0; j < ports.size(); j++) {
//             int port = ports[j];
//             std::cout << port << " ";
//         }
//         std::cout << "\n";

//         intCharMap errorPages = config.getErrorPages(i);
//         std::cout << "Error Pages:\n";
//         intCharMap::const_iterator it;
//         for (it = errorPages.begin(); it != errorPages.end(); ++it) {
//             std::cout << "HTTP Status: " << it->first << ", File: " << it->second << "\n";
//         }

//         locationVector locations = config.getLocations(i);
//         std::cout << "Locations:\n";
//         for (size_t k = 0; k < locations.size(); k++) {
//             const Location& location = locations[k];
//             std::cout << "Path: " << location.getPath() << "\n";
//             // Agrega más información sobre la ubicación si es necesario
//         }

//         std::cout << "--------------------------------\n";
//     }
// }

int run(const string &path)
{
	cout << " vamos a intentar correrlo con archivo de configuración " << '"' << path << '"' << endl;
	try
	{
		Config	config(path);

		WebServer	werbserver(config);
		signal(SIGINT, WebServer::signalHandler);
		signal(SIGTERM, WebServer::signalHandler);

		werbserver.serverLoop();
	}
	catch(const std::exception& e)
	{
		std::cerr << '\n' << "Runtime error:\n" << e.what() << '\n';
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int main(int argc, char const *argv[])
{
	cout << " has introducido " << argc << " argumentos en " << argv[0] << endl;
	if (argc > 2)
	{
		cerr << "❌Error: demasiados argumentos de entrada, espero uno o niguno" << endl;
		return(EXIT_FAILURE);
	}
	if (argc == 2)
		return(run(argv[1]));
	cout << " vamos a usar el path por defecto " << '"' << defaultPath << '"' << endl;

	return(run(defaultPath));
}
