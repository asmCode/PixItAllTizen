#ifndef HTTPCOMMUNICATION_H_
#define HTTPCOMMUNICATION_H_

#include "IHttpCommunicationObserver.h"
#include <FNet.h>
#include <string>

class HttpCommunication : public Tizen::Net::Http::IHttpTransactionEventListener
{
public:
	HttpCommunication(const std::string& hostAddress);
	virtual ~HttpCommunication();

	void AddObserver(IHttpCommunicationObserver* observer);
	void SendRequest(const std::string& address);

	bool IsBusy() const;

private:
	IHttpCommunicationObserver* m_observer;

	Tizen::Net::Http::HttpSession* m_session;
	Tizen::Net::Http::HttpTransaction* m_transaction;

	bool m_isBusy;

	void OnTransactionReadyToRead(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, int availableBodyLen);
	void OnTransactionAborted(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, result r);
	void OnTransactionReadyToWrite(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, int recommendedChunkSize);
	void OnTransactionHeaderCompleted(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, int headerLen, bool bAuthRequired);
	void OnTransactionCompleted(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction);
	void OnTransactionCertVerificationRequiredN(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, Tizen::Base::String* pCert);
};

#endif /* HTTPCOMMUNICATION_H_ */
