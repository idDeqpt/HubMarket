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
	request.headers["Connection"] = "close";
	//request.headers["Cache-Control"] = "max-age=0";
	//request.headers["sec-ch-ua"] = "\"Chromium\";v=\"128\", \"Not;A=Brand\";v=\"24\", \"Opera GX\";v=\"114\"";
	//request.headers["sec-ch-ua-mobile"] = "?0";
	//request.headers["sec-ch-ua-platform"] = "\"Windows\"";
	//request.headers["Upgrade-Insecure-Requests"] = "1";
	//request.headers["User-Agent"] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/128.0.0.0 Safari/537.36 OPR/114.0.0.0 (Edition Yx GX 03)";
	//request.headers["Accept"] = "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7";
	//request.headers["Sec-Fetch-Site"] = "none";
	//request.headers["Sec-Fetch-Mode"] = "navigate";
	//request.headers["Sec-Fetch-User"] = "?1";
	//request.headers["Sec-Fetch-Dest"] = "document";
	//request.headers["Accept-Encoding"] = "gzip, deflate, br, zstd";
	//request.headers["Accept-Language"] = "ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7";

	std::cout << "|" << request.toString() << "|\n";

	Network::TCPClient client;
	client.connect(request.headers["Host"], 80);//((url.getScheme() == "http") ? 80 : 443));
	client.send(request.toString());
	return client.recv();
}