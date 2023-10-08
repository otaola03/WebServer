#include "./includes/webserver.h"


int main()
{
	Server server;
	int	new_fd;

	 while(1) {  // main accept() loop
		std::cout << "Waiting to accept\n";
		new_fd = server.acceptConnection();
		
		char buf[1000];
		int numbytes;
		if ((numbytes = recv(new_fd, buf, 1000-1, 0)) == -1)
		{
        	perror("recv");
        	exit(1);
    	}
		buf[numbytes] = '\0';
		/* std::cout << buf << "\n\n"; */

		std::string str(buf);
		HttpRequest	request(str);

        close(new_fd);  // parent doesn't need this
		break;
    }

    return 0;
}
