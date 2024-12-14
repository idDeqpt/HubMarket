#ifndef NETWORK_URL_CLASS
#define NETWORK_URL_CLASS

#include <string>
#include <vector>

#include "Address.hpp"
#include "../Dictionary.hpp"


namespace Network
{
	class URI
	{
	public:
		URI();
		URI(std::string uri);

		unsigned int getLength();
		Dictionary<std::string, std::string>& getParamsPtr();

		std::string toString(bool with_params = true);

		std::string& operator[](int index);

	protected:
		std::vector<std::string> path;
		Dictionary<std::string, std::string> params;
	};

	class URL
	{
	public:
		URL();
		URL(std::string scheme, Address host);
		URL(std::string scheme, Address host, URI uri);
		URL(std::string url);

		void setScheme(std::string new_scheme);
		void setHost(Address new_host);
		void setURI(URI new_uri);

		std::string getScheme();
		Address getHost();
		URI getURI();

		std::string toString();

	protected:
		std::string scheme;
		Address host;
		URI uri;
	};
}

#endif //NETWORK_URL_CLASS