#include "HttpCommunication.h"
#include <Utils/Log.h>
#include <Utils/StringUtils.h>

#include <FBase.h>

using namespace Tizen::Net::Http;
using namespace Tizen::Base;
using namespace Tizen::Base::Runtime;

HttpCommunication::HttpCommunication(const std::string& hostAddress) :
	m_transaction(NULL)
{
	m_session = new HttpSession();
	result r = m_session->Construct(NET_HTTP_SESSION_MODE_NORMAL, NULL, hostAddress.c_str(), NULL);
	if (r != E_SUCCESS)
	{
		delete m_session;
		m_session = NULL;
	}
}

HttpCommunication::~HttpCommunication()
{
	if (m_session != NULL)
	{
		m_session->CloseAllTransactions();
		delete m_session;
	}
}

void HttpCommunication::AddObserver(IHttpCommunicationObserver* observer)
{
	m_observer = observer;
}

void HttpCommunication::SendRequest(const std::string& address)
{
	if (m_session == NULL)
		return;

	m_transaction = m_session->OpenTransactionN();
	if (m_transaction == NULL)
		return;

	m_transaction->AddHttpTransactionListener(*this);

	HttpRequest* request = m_transaction->GetRequest();
	if (request == NULL)
		return;

	request->SetUri(address.c_str());
	request->SetMethod(NET_HTTP_METHOD_GET);

	m_transaction->Submit();
}

bool HttpCommunication::IsBusy() const
{
	return m_transaction != NULL;
}

void HttpCommunication::OnTransactionReadyToRead(HttpSession& httpSession, HttpTransaction& httpTransaction, int availableBodyLen)
{
	Log::LogT("OnTransactionReadyToRead");
}

void HttpCommunication::OnTransactionAborted(HttpSession& httpSession, HttpTransaction& httpTransaction, result r)
{
	m_session->CloseTransaction(*m_transaction);
	m_transaction = NULL;

	Log::LogT("OnTransactionAborted");
}

void HttpCommunication::OnTransactionReadyToWrite(HttpSession& httpSession, HttpTransaction& httpTransaction, int recommendedChunkSize)
{
	Log::LogT("OnTransactionReadyToWrite");
}

void HttpCommunication::OnTransactionHeaderCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction, int headerLen, bool bAuthRequired)
{
	Log::LogT("OnTransactionHeaderCompleted");
}

void HttpCommunication::OnTransactionCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction)
{
	HttpResponse* response = httpTransaction.GetResponse();
	if (response == NULL)
		return;

	ByteBuffer* dataBuffer = response->ReadBodyN();
	if (dataBuffer == NULL)
		return;

	std::string data(reinterpret_cast<char*>(dataBuffer->GetPointer()), dataBuffer->GetCapacity());

	if (m_observer != NULL)
		m_observer->Response(this, response->GetHttpStatusCode(), data);

	m_session->CloseTransaction(*m_transaction);
	m_transaction = NULL;
}

void HttpCommunication::OnTransactionCertVerificationRequiredN(HttpSession& httpSession, HttpTransaction& httpTransaction, Tizen::Base::String* pCert)
{
	Log::LogT("OnTransactionCertVerificationRequiredN");
}
