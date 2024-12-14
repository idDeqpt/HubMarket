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
	if (uri.empty() || (uri == "/"))
	{
		path.push_back("/");
		return;
	}

	if (uri[0] == '/')
		uri.erase(uri.begin());

	int params_i = uri.find("?");
	if (params_i == std::string::npos)
	{
		uri += "/";
		params_i = uri.length();
	}
	else
		uri.insert(uri.begin() + params_i++, '/');

	int pointer_begin = 0;
	int pointer_end;
	for (pointer_end = uri.find("/", pointer_begin + 1); (pointer_end != std::string::npos) && (pointer_end < params_i); pointer_end = uri.find("/", pointer_begin + 1))
	{
		path.push_back(uri.substr(pointer_begin, pointer_end - pointer_begin));
		pointer_begin = pointer_end + 1;
	}

	if (params_i == uri.length())
		return;

	uri += "&";
	pointer_begin = params_i + 1;
	for (pointer_end = uri.find("&", pointer_begin + 1); pointer_end != std::string::npos; pointer_end = uri.find("&", pointer_begin + 1))
	{
		int equel_pos = uri.find("=", pointer_begin);
		params[uri.substr(pointer_begin, equel_pos - pointer_begin)] = uri.substr(equel_pos + 1, pointer_end - equel_pos - 1);
		pointer_begin = pointer_end + 1;
	}
}


unsigned int Network::URI::getLength()
{
	return path.size();
}

Dictionary<std::string, std::string>& Network::URI::getParamsPtr()
{
	return params;
}


std::string Network::URI::toString()
{
	std::string result;
	for (unsigned int i = 0; i < path.size(); i++)
		result += "/" + path[i];

	if (params.getSize() > 0)
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
	path = "";
}

Network::URL::URL(std::string scheme, Address host)
{
	this->scheme = scheme;
	this->host = host;
	this->path = "";
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
	{
		path = "";
		return;
	}
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
	std::string params_str = "";
	if (params.getSize() > 0)
	{
		params_str += "?";
		for (unsigned int i = 0; i < params.getSize(); i++)
			params_str += params.getItemPtr(i).key + "=" + params.getItemPtr(i).value + "&";
		params_str.pop_back();
	}
	return scheme + "://" + host.toString() + uri.toString;
}