#include "HTTP.hpp"

#include "../Dictionary.hpp"


Network::HTTP::HTTP()
{
	http_version = "";
}



Network::HTTPRequest::HTTPRequest() : HTTP()
{

}

Network::HTTPRequest::HTTPRequest(std::string request) : HTTP()
{
	int pointer_front = request.find(" ");
	int pointer_back = 0;
	method = request.substr(0, pointer_front);
	pointer_back = pointer_front + 1;

	pointer_front = request.find(" ", pointer_back + 1);
	uri = request.substr(pointer_back, pointer_front - pointer_back);
	pointer_back = pointer_front + 1;

	pointer_front = request.find("\r\n", pointer_back + 1);
	http_version = request.substr(pointer_back, pointer_front - pointer_back);
	pointer_back = pointer_front + 2;

	for (pointer_front = request.find("\r\n", pointer_back + 1); pointer_front != std::string::npos; pointer_front = request.find("\r\n", pointer_back + 1))
	{
		int sep_pos = request.find(": ", pointer_back);
		headers[request.substr(pointer_back, sep_pos - pointer_back)] = request.substr(sep_pos + 2, pointer_front - sep_pos - 2);
		pointer_back = pointer_front;
	}
}

std::string Network::HTTPRequest::toString()
{
	std::string request = method + " " + uri + " " + http_version + "\r\n";
	for (unsigned int i = 0; i < headers.getSize(); i++)
		request += headers.getItemPtr(i).key + ": " + headers.getItemPtr(i).value + "\r\n";
	return request;
}