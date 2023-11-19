#include "HttpResponse.hpp"

HttpResponse::HttpResponse()
{
}

HttpResponse::HttpResponse(const HttpResponse& toCopy)
{
	(void)toCopy;
}

HttpResponse::~HttpResponse()
{
}

HttpResponse& HttpResponse::operator=(const HttpResponse& toAssign)
{
	(void)toAssign;
	return *this;
}

