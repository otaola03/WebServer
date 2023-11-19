#ifndef HTTRESPONSE_HPP

# define HTTRESPONSE_HPP

class HttpResponse
{
	private:
	public:
		HttpResponse();
		HttpResponse(const HttpResponse& toCopy);
		~HttpResponse();

		HttpResponse& operator=(const HttpResponse& toCopy);
};

#endif
