#ifndef NETWORK_HTTP_CLASS
#define NETWORK_HTTP_CLASS

#include <string>

#include "../Dictionary.hpp"


namespace Network
{
	class HTTP
	{
	public:
		std::string http_version;
		Dictionary<std::string, std::string> headers;

		HTTP();

		virtual std::string toString() = 0;
	};

	class HTTPRequest : public HTTP
	{
	public:
		std::string method;
		std::string uri;

		HTTPRequest();
		HTTPRequest(std::string request);
		std::string toString();
	};

	class HTTPResponse : public HTTPRequest
	{
	public:
		std::string status_code;
		std::string status_comment;
		std::string body;

		HTTPResponse();
		HTTPResponse(std::string response);
		std::string toString();

	};
}

#endif //NETWORK_HTTP_CLASS