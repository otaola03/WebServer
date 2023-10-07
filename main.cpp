#include "./includes/webserver.h"


int main()
{
	Server server;
	int	new_fd;

	 while(1) {  // main accept() loop
		std::cout << "Waitin to accept\n";
		new_fd = server.acceptConnection();
		if (send(new_fd, "Hello, world!", 13, 0) == -1)
			perror("send");

        close(new_fd);  // parent doesn't need this
		/* break; */
    }

    return 0;
}
