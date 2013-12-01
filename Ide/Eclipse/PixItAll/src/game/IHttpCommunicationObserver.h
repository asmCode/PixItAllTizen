#ifndef IHTTPCOMMUNICATION_H_
#define IHTTPCOMMUNICATION_H_

#include <string>

class HttpCommunication;

class IHttpCommunicationObserver
{
public:
	virtual ~IHttpCommunicationObserver() {}

	virtual void Timeount() = 0;
	virtual void Response(HttpCommunication* http, int httpCode, const std::string& data) = 0;
};


#endif /* IHTTPCOMMUNICATION_H_ */
