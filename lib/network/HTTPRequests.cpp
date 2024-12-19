#include "HTTPRequests.hpp"

#include <string>
#include <iostream>

#include "URL.hpp"
#include "HTTP.hpp"
#include "TCPClient.hpp"


std::string Network::Requests::get(std::string url)
{
	return Network::Requests::get(Network::URL(url)).toString();
}

Network::HTTPResponse Network::Requests::get(Network::URL url)
{
	std::cout << url.toString() << std::endl;
	HTTPRequest request;

	request.start_line["method"] = "GET";
	request.start_line["uri"] = url.getURI().toString();
	request.start_line["http-version"] = "HTTP/1.1";

	request.headers["Host"] = url.getHost();

	std::cout << "|" << request.toString() << "|\n";

	Network::TCPClient client;
	client.connect(request.headers["Host"], ((url.getScheme() == "http") ? 80 : 443));
	client.send(request.toString());
	return client.recv();
}