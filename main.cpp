#include "./includes/webserver.h"
// #include "srcs/classes/Server.hpp"


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

int main()
{
	WebServer	webserver;

	webserver.serverLoop();
	/* Port	port1(80); */
	/* Port	port2(85); */
}
