#include "URL.hpp"

#include <string>

#include "Address.hpp"
#include "../Dictionary.hpp"


Network::URL::URL()
{
	scheme = "http";
	host = Address(IP(127, 0, 0, 1), 80);
	path = "";
}

Network::URL::URL(std::string scheme, Address host)
{
	this->scheme = scheme;
	this->host = host;
	this->path = "";
}

Network::URL::URL(std::string scheme, Address host, std::string path)
{
	this->scheme = scheme;
	this->host = host;
	this->path = path;
}

Network::URL::URL(std::string url)
{
	int pointer_back = 0;
	int pointer_front = url.find("://");
	scheme = url.substr(0, pointer_front);
	pointer_back = pointer_front + 3;

	pointer_front = url.find("/", pointer_back + 1);
	host = Address(url.substr(pointer_back, pointer_front - pointer_back));
	if (pointer_front == std::string::npos)
	{
		path = "";
		return;
	}
	pointer_back = pointer_front + 1;

	pointer_front = url.find("?", pointer_back + 1);
	if (pointer_front == std::string::npos)
	{
		path = url.substr(pointer_back, url.length() - pointer_back);
		return;
	}

	path = url.substr(pointer_back, pointer_front - pointer_back);
	pointer_back = pointer_front + 1;
	url += "&";
	for (pointer_front = url.find("&", pointer_back + 1); pointer_front != std::string::npos; pointer_front = url.find("&", pointer_back + 1))
	{
		int equel_pos = url.find("=", pointer_back);
		params[url.substr(pointer_back, equel_pos - pointer_back)] = url.substr(equel_pos + 1, pointer_front - equel_pos - 1);
		pointer_back = pointer_front;
	}
}


void Network::URL::setScheme(std::string new_scheme)
{
	scheme = new_scheme;
}

void Network::URL::setHost(Address new_host)
{
	host = new_host;
}

void Network::URL::setPath(std::string new_path)
{
	path = new_path;
}


std::string Network::URL::getScheme()
{
	return scheme;
}

Network::Address Network::URL::getHost()
{
	return host;
}

std::string Network::URL::getPath()
{
	return path;
}

Dictionary<std::string, std::string>& Network::URL::getParamsPtr()
{
	return params;
}


std::string Network::URL::toString()
{
	std::string params_str = "";
	if (params.getSize() > 0)
	{
		params_str += "?";
		for (unsigned int i = 0; i < params.getSize(); i++)
			params_str += params.getItemPtr(i).key + "=" + params.getItemPtr(i).value + "&";
		params_str.pop_back();
	}
	return scheme + "://" + host.toString() + "/" + path + params_str;
}