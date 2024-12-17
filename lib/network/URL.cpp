#include "URL.hpp"

#include <string>
#include <vector>
#include <iostream>

#include "Address.hpp"
#include "../Dictionary.hpp"


Network::URI::URI()
{
	path.push_back("/");
}

Network::URI::URI(std::string uri)
{
	int params_i = uri.find("?");

	std::string path_str = (params_i == std::string::npos) ? uri : uri.substr(0, params_i);

	if (path_str.empty() || (path_str == "/"))
		path.push_back("");
	else
	{
		path_str += "/";
		if (path_str[0] == '/')
			path_str.erase(path_str.begin());

		int pointer_begin = 0;
		int pointer_end;
		for (pointer_end = path_str.find("/", pointer_begin + 1); (pointer_end != std::string::npos) && (pointer_end < params_i); pointer_end = path_str.find("/", pointer_begin + 1))
		{
			path.push_back(path_str.substr(pointer_begin, pointer_end - pointer_begin));
			pointer_begin = pointer_end + 1;
		}
	}

	if (params_i == uri.length())
		return;

	uri += "&";
	int pointer_begin = params_i + 1;
	for (int pointer_end = uri.find("&", pointer_begin + 1); pointer_end != std::string::npos; pointer_end = uri.find("&", pointer_begin + 1))
	{
		int equel_pos = uri.find("=", pointer_begin);
		params[uri.substr(pointer_begin, equel_pos - pointer_begin)] = uri.substr(equel_pos + 1, pointer_end - equel_pos - 1);
		pointer_begin = pointer_end + 1;
		std::cout << pointer_begin << " " << pointer_end << std::endl;
	}
	for (unsigned int i = 0; i < params.getSize(); i++)
		std::cout << params.getItemPtr(i).key << " " << params.getItemPtr(i).value << std::endl;
}


unsigned int Network::URI::getLength()
{
	return path.size();
}

Dictionary<std::string, std::string>& Network::URI::getParamsPtr()
{
	return params;
}


std::string Network::URI::toString(bool with_params)
{
	std::string result;
	for (unsigned int i = 0; i < path.size(); i++)
		result += "/" + path[i];

	if (with_params && (params.getSize() > 0))
	{
		result += "?";
		for (unsigned int i = 0; i < params.getSize(); i++)
			result += params.getItemPtr(i).key + "=" + params.getItemPtr(i).value + "&";
		result.pop_back();
	}

	return result;
}


std::string& Network::URI::operator[](int index)
{
	return path[index];
}



Network::URL::URL()
{
	scheme = "http";
	host = Address(IP(127, 0, 0, 1), 80);
	uri = URI();
}

Network::URL::URL(std::string scheme, Address host)
{
	this->scheme = scheme;
	this->host = host;
	this->uri = URI();
}

Network::URL::URL(std::string scheme, Address host, URI uri)
{
	this->scheme = scheme;
	this->host = host;
	this->uri = uri;
}

Network::URL::URL(std::string url)
{
	int pointer_begin = 0;
	int pointer_end = url.find("://");
	scheme = url.substr(0, pointer_end);
	pointer_begin = pointer_end + 3;

	pointer_end = url.find("/", pointer_begin + 1);
	host = Address(url.substr(pointer_begin, pointer_end - pointer_begin));
	if (pointer_end == std::string::npos)
		return;

	pointer_begin = pointer_end + 1;
	uri = URI(url.substr(pointer_begin, pointer_end - pointer_begin));
}


void Network::URL::setScheme(std::string new_scheme)
{
	scheme = new_scheme;
}

void Network::URL::setHost(Address new_host)
{
	host = new_host;
}

void Network::URL::setURI(URI new_uri)
{
	uri = new_uri;
}


std::string Network::URL::getScheme()
{
	return scheme;
}

Network::Address Network::URL::getHost()
{
	return host;
}

Network::URI Network::URL::getURI()
{
	return uri;
}


std::string Network::URL::toString()
{
	return scheme + "://" + host.toString() + uri.toString();
}