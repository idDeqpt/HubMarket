#include "HTTP.hpp"

#include <string>

#include "../Dictionary.hpp"


Network::HTTP::HTTP()
{
	body = "";
}

Network::HTTP::HTTP(std::string message)
{
	start_line = {
		{"0", ""},
		{"1", ""},
		{"2", ""}
	};

	int body_pos = (message.find("\r\n\r\n\r\n") == std::string::npos) ? std::string::npos : (message.find("\r\n\r\n") + 6);
	int pointer_begin = 0;
	int pointer_end = message.find(" ");
	start_line["0"] = message.substr(0, pointer_end);
	pointer_begin = pointer_end + 1;

	pointer_end = message.find(" ", pointer_begin + 1);
	start_line["1"] = message.substr(pointer_begin, pointer_end - pointer_begin);
	pointer_begin = pointer_end + 1;

	pointer_end = message.find("\r\n", pointer_begin + 1);
	start_line["2"] = message.substr(pointer_begin, pointer_end - pointer_begin);
	pointer_begin = pointer_end + 2;

	int stop_point = (body_pos == std::string::npos) ? message.length() : (body_pos - 6);
	for (pointer_end = message.find("\r\n", pointer_begin + 1); (pointer_end != std::string::npos) && (pointer_begin < stop_point); pointer_end = message.find("\r\n", pointer_begin + 1))
	{
		int sep_pos = message.find(": ", pointer_begin);
		headers[message.substr(pointer_begin, sep_pos - pointer_begin)] = message.substr(sep_pos + 2, pointer_end - sep_pos - 2);
		pointer_begin = pointer_end + 2;
	}

	body = (body_pos != std::string::npos) ? message.substr(body_pos, message.length() - body_pos) : "";
}

std::string Network::HTTP::toString()
{
	std::string message = start_line.getItemPtr(0).value + " " + start_line.getItemPtr(1).value + " " + start_line.getItemPtr(2).value + "\r\n";
	for (unsigned int i = 0; i < headers.getSize(); i++)
		message += headers.getItemPtr(i).key + ": " + headers.getItemPtr(i).value + "\r\n";
	return message + ((body.length()) ? ("\r\n" + body) : "");
}



Network::HTTPRequest::HTTPRequest() : HTTP()
{

}

Network::HTTPRequest::HTTPRequest(std::string request) : HTTP(request)
{
	start_line.getItemPtr(0).key = "method";
	start_line.getItemPtr(1).key = "uri";
	start_line.getItemPtr(2).key = "http-version";
}



Network::HTTPResponse::HTTPResponse() : HTTP()
{
	
}

Network::HTTPResponse::HTTPResponse(std::string response) : HTTP(response)
{
	start_line.getItemPtr(0).key = "http-version";
	start_line.getItemPtr(1).key = "status-code";
	start_line.getItemPtr(2).key = "status-comment";
}