#ifndef BASE_H
#define BASE_H

// Include Libary
#pragma comment(lib, "WS2_32.lib")

// needed for sockets
#include <winsock2.h>	
#include "Structs.h"
#include "CircularBuffer.h"

class NetBasement;

DWORD WINAPI ListenFunc(LPVOID lpParam);
DWORD WINAPI TalkFunc(LPVOID lpParam);

// ****************************************************************************
// Low level network class
// ****************************************************************************
class NetBasement
{
	friend DWORD WINAPI ::ListenFunc(LPVOID lpParam);
	friend DWORD WINAPI ::TalkFunc(LPVOID lpParam);
	//---------------------------------
	// Network Basement Init DeInit
	//---------------------------------
private:
	NetBasement();
	void Init();
public:
	NetBasement(const CircularBuffer& _outbox);
	NetBasement(const CircularBuffer& _outbox, IP_ADDRESS_STRING _sourceIP);	
	~NetBasement();

	//---------------------------------
	// Socket Management
	//---------------------------------
private:
	void createSockets();						// Create a socket
	void bindSocket();							// Bind Socket, Listen, Accept
	void acceptSocket();						// Accept Connection on Socket
	void connectSocket();						// Connect To Accepting Sender
	void closeSockets();						// Close socket	

	//---------------------------------
	// Buffer Management
	//---------------------------------
private:
	void startThreads();
public:
	const CircularBuffer& getInbox() const;
	//---------------------------------
	// Messages
	//---------------------------------
private:
	bool sendMessage(JWL_NET_MSG _message);		// Send Message
	JWL_NET_MSG receiveFromSender();			// Recieve Mesage


private:
	UINT	m_Sendport;							// Sending Port
	UINT	m_Recvport;							// Recption Port
	UINT	m_lastError;						// Last Error

	WSADATA* m_wsData;							// Info on network capabilities, Used when starting winsock

	SOCKET m_rSocket;							// Reception Socket
	SOCKET m_sSocket;							// Sender Socket

	CircularBuffer m_outbox;					// Outgoing Messages Storage
	CircularBuffer m_inbox;						// Incoming Messages Storage

	HANDLE m_tListen;							// Listening Thread ID
	HANDLE m_tTalk;								// Talking Thread ID

	IP_ADDRESS_STRING m_sourceIP;				// Other sides IP
};

#endif