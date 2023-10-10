#ifndef WEBSERVER_HPP

# define WEBSERVER_HPP

class WebServer
{
	private:
	public:
		WebServer();
		WebServer(const WebServer& toCopy);
		~WebServer();

		void	serverLoop();

		WebServer& operator=(const WebServer& toAssign);
};

#endif
