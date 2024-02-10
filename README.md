# Webserver
*A small replica of nginx.*


## The Challenge
The aim of this project is to construct an HTTP web server from scratch, compatible with C++98 standards. The web server is designed to process HTTP GET, HEAD, POST, PUT, and DELETE requests. It can serve static files from a designated root directory and handle dynamic content through CGI. Moreover, it is equipped to manage multiple client connections simultaneously, thanks to the utilization of `kqueue()`.


## Usage
```bash
make
./webserver [Config File] ## leave empty to use the default configuration.
```
