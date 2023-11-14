#include "./includes/webserver.h"


/* int main() */
/* { */
/* 	Port port(1090); */
/* 	int	new_fd; */

/* 	 while(1) {  // main accept() loop */
/* 		std::cout << "Waiting to accept\n"; */
/* 		port.activatePort(); */
/* 		new_fd = port.acceptConnection(); */
		
/* 		char buf[1000]; */
/* 		int numbytes; */
/* 		if ((numbytes = recv(new_fd, buf, 1000-1, 0)) == -1) */
/* 		{ */
/*         	perror("recv"); */
/*         	exit(1); */
/*     	} */
/* 		buf[numbytes] = '\0'; */
/* 		/1* std::cout << buf << "\n\n"; *1/ */

/* 		std::string str(buf); */
/* 		HttpRequest	request(str); */

/*         close(new_fd);  // parent doesn't need this */
/* 		break; */
/*     } */

/*     return 0; */
/* } */

int run(const string &path)
{
	cout << " vamos a intentar correrlo con archivo de configuración " << '"' << path << '"' << endl;
	try
	{
		Config	config(path);

		WebServer	werbserver(config);
		// werbserver.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << '\n' << "Error:\n" << e.what() << '\n';
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
