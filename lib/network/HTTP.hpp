#ifndef NETWORK_HTTP_CLASS
#define NETWORK_HTTP_CLASS

#include <string>

#include "../Dictionary.hpp"


namespace Network
{
	class HTTP
	{
	public:
		Dictionary<std::string, std::string> headers;
		Dictionary<std::string, std::string> start_line;
		std::string body;

		HTTP();
		HTTP(std::string message);
		std::string toString();
	};

	class HTTPRequest : public HTTP
	{
	public:
		HTTPRequest();
		HTTPRequest(std::string request);
	};

	class HTTPResponse : public HTTP
	{
	public:
		HTTPResponse();
		HTTPResponse(std::string response);

	};
}

#endif //NETWORK_HTTP_CLASS