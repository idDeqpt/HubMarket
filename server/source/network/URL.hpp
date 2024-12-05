#ifndef NETWORK_URL_CLASS
#define NETWORK_URL_CLASS

#include <string>

#include "Address.hpp"
#include "../Dictionary.hpp"


namespace Network
{
	class URL
	{
	public:
		URL();
		URL(std::string scheme, Address host);
		URL(std::string scheme, Address host, std::string path);
		URL(std::string url);

		void setScheme(std::string new_scheme);
		void setHost(Address new_host);
		void setPath(std::string new_path);

		std::string getScheme();
		Address getHost();
		std::string getPath();
		Dictionary<std::string, std::string>& getParamsPtr();

		std::string toString();

	protected:
		std::string scheme;
		Address host;
		std::string path;
		Dictionary<std::string, std::string> params;
	};
}

#endif //NETWORK_URL_CLASS