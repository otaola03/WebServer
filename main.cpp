#include <iostream>
using	std::cerr;
using	std::cout;
using	std::endl;

#include <string>
using	std::string;

const string defaultPath = "config/default.yml";

#include "./srcs/classes/Config.hpp"
#include "./srcs/classes/WebServer.hpp"

int run(const string &path)
{
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
	if (argc > 2)
	{
		cerr << "❌Error: demasiados argumentos de entrada, espero uno o niguno" << endl;
		return(EXIT_FAILURE);
	}
	if (argc == 2)
		return(run(argv[1]));
	return(run(defaultPath));
}