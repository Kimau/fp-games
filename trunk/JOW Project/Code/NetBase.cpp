#include "NetBase.h"

//######################################################################################
//######################################################################################
//						Construct / Deconstruct
//######################################################################################
//######################################################################################
NetBasement::NetBasement() : m_inbox(false), m_outbox(false)
{
}
///////////////////////////////////////////////////////////////////////////////
// Game Starter
///////////////////////////////////////////////////////////////////////////////
NetBasement::NetBasement(const CircularBuffer& _outbox): 
m_inbox(true),
m_outbox(_outbox)
{
	memset(m_sourceIP.String,0,sizeof(char)*16);
	m_Sendport		 = NET_PORT_SERV_OUT;
	m_Recvport		 = NET_PORT_CLIENT_OUT;

	Init();
	//--------------------------------
	// Create Sockets and Wait
	//--------------------------------
	createSockets();	if(m_lastError) return;
	bindSocket();		if(m_lastError) return;
	acceptSocket();		if(m_lastError) return;	// This line will pause until connection is made
	Sleep(2000);
	connectSocket();	if(m_lastError) return;	// This line will pause until connection is made

	startThreads();								// Start Threads
}
///////////////////////////////////////////////////////////////////////////////
// Game Joiner
///////////////////////////////////////////////////////////////////////////////
NetBasement::NetBasement(const CircularBuffer& _outbox, IP_ADDRESS_STRING _sourceIP) :
m_sourceIP(_sourceIP), 
m_inbox(true),
m_outbox(_outbox)
{
	m_Sendport		 = NET_PORT_CLIENT_OUT;
	m_Recvport		 = NET_PORT_SERV_OUT;

	Init();
	//--------------------------------
	// Create Sockets and Connect
	//--------------------------------
	createSockets();	if(m_lastError) return;
	bindSocket();		if(m_lastError) return;
	connectSocket();	if(m_lastError) return;	// This line will pause until connection is made
	acceptSocket();		if(m_lastError) return;	// This line will pause until connection is made	

	startThreads();								// Start Threads
}

void NetBasement::Init()
{
	m_lastError		 = 0;
	m_rSocket		 = 0;
	m_sSocket		 = 0;
	m_tListen		 = 0;
	m_tTalk			 = 0;
	m_wsData		 = new WSAData;

	//--------------------------------
	// Start Up WinSock
	// Give it Version(2.2)
	// Pointer to Dump Data
	// Returns Zero if NO errors
	//--------------------------------
	if(WSAStartup(MAKEWORD(2, 2), m_wsData))
	{
		// Errors Occured Handle
		delete m_wsData;
		m_wsData = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Destructor.
///////////////////////////////////////////////////////////////////////////////
NetBasement::~NetBasement()
{
	closeSockets();
	if(m_wsData)
	{
		WSACleanup();
		delete m_wsData;
	}
	if(m_tListen)		TerminateThread(m_tListen,0);
	if(m_tTalk)			TerminateThread(m_tTalk,0);
}

//######################################################################################
//######################################################################################
//						Socket Management
//######################################################################################
//######################################################################################
///////////////////////////////////////////////////////////////////////////////
// create a socket.
///////////////////////////////////////////////////////////////////////////////
void NetBasement::createSockets()
{
	if(m_lastError != 0)	return;					// Check if Errors Pending
	if(m_wsData == 0)		return;					// Check if WinSock Active
	if(m_rSocket != 0)		return;					// Check if ALREADY Socket Created
	if(m_sSocket != 0)		return;					// Check if ALREADY Socket Created

	//-----------------------------
	// Create Sockets
	// AF_INET		= IP v4 Packet
	// SOCK_STREAM	= Steam Data
	// PROTOCALL	= 
	//-----------------------------
	int _tempR = (int)socket(AF_INET, SOCK_STREAM, 0);
	int _tempS = (int)socket(AF_INET, SOCK_STREAM, 0);

	//--------------------------------
	// Error Handling
	//--------------------------------
	if((_tempR == SOCKET_ERROR) || (_tempS == SOCKET_ERROR))
	{
		m_lastError = WSAGetLastError();
		return;
	}
	m_rSocket = _tempR;
	m_sSocket = _tempS;
}


///////////////////////////////////////////////////////////////////////////////
// close socket.
///////////////////////////////////////////////////////////////////////////////
void NetBasement::closeSockets()
{
	if(m_lastError != 0)	return;					// Check if Errors Pending
	if(m_wsData == 0)		return;					// Check if WinSock Active
	if(m_rSocket == 0)		return;					// Check if Socket Created
	if(m_sSocket == 0)		return;					// Check if Socket Created

	//-----------------------------
	// Close Socket
	//--------------------------------
	if((closesocket(m_rSocket) == SOCKET_ERROR) || (closesocket(m_sSocket) == SOCKET_ERROR))
	{
		m_lastError = WSAGetLastError();
		return;
	}
	m_rSocket = 0;
	m_sSocket = 0;
}


///////////////////////////////////////////////////////////////////////////////
// bind socket to port, listen for connections, and accept them.
///////////////////////////////////////////////////////////////////////////////
void NetBasement::bindSocket()
{
	if(m_lastError != 0)	return;					// Check if Errors Pending
	if(m_wsData == 0)		return;					// Check if WinSock Active
	if(m_rSocket == 0)		return;					// Check if Socket Created

	//------------------------------
	// Parameter Struct
	//
	// + AF_INET - IPv4
	// + Port - Htons Orders Bytes
	// + What Adress Acepted
	// + Full Up Info
	//------------------------------
	sockaddr_in _sockAddress = {AF_INET, htons(m_Recvport),INADDR_ANY, {0}};

	//------------------------------
	// Bind Socket
	//------------------------------
	if(bind(m_rSocket ,(sockaddr*)&_sockAddress, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		m_lastError = WSAGetLastError();
		return;
	}

	//------------------------------
	// Listen
	//------------------------------
	if(listen(m_rSocket, 1) == SOCKET_ERROR)
	{
		m_lastError = WSAGetLastError();
		return;
	}
}
///////////////////////////////////////////////////////////////////////////////
// Await connection
///////////////////////////////////////////////////////////////////////////////
void NetBasement::acceptSocket()
{
	if(m_lastError != 0)	return;					// Check if Errors Pending
	if(m_wsData == 0)		return;					// Check if WinSock Active
	if(m_rSocket == 0)		return;					// Check if Socket Created

	//--------------------------------
	// Accept Socket
	//--------------------------------
	// TODO :: TEST NULL
	sockaddr _SockParam;
	int _tempV = (int)accept(m_rSocket, &_SockParam, 0);

	//--------------------------------
	// Error Handling
	//--------------------------------
	if(_tempV == SOCKET_ERROR)
	{
		m_lastError = WSAGetLastError();
		return;
	}	
	m_rSocket = _tempV;

	if(m_sourceIP.String[0] == 0)
	{
		//--------------------------------
		// Get IP address of connector
		//--------------------------------
		SOCKADDR _sockAdd;		memset(&_sockAdd,0,sizeof(_sockAdd));
		int _namelen = sizeof(_sockAdd);

		if(getpeername(m_rSocket,&_sockAdd,&(_namelen)) == SOCKET_ERROR)
		{
			m_lastError = WSAGetLastError();
			return;
		}
		//--------------------------------
		// Convert it from weird format
		// to normal format
		//--------------------------------
		char _ip[16] = {0};

        for(UINT c = 2; c < 6; ++c)
		{
			char _buff[3] = {0};
			itoa((int)_sockAdd.sa_data[c],_buff,10);
			strncat(_ip,_buff,3);
			if(c != 5) strncat(_ip,".",1);
		}
		//--------------------------------
		// Set It
		//--------------------------------
		memcpy(m_sourceIP.String,_ip,sizeof(char)*16);
	}
}
///////////////////////////////////////////////////////////////////////////////
// connect to listening sender.
///////////////////////////////////////////////////////////////////////////////
void NetBasement::connectSocket()
{
	if(m_lastError != 0)	return;					// Check if Errors Pending
	if(m_wsData == 0)		return;					// Check if WinSock Active
	if(m_sSocket == 0)		return;					// Check if his Sockets NOT already create

	//------------------------------
	// Parameter Struct
	//
	// + AF_INET - IPv4
	// + Port - Htons Orders Bytes
	// + What Adress Acepted
	// + Full Up Info
	//------------------------------
	sockaddr_in _sockAddress;
	_sockAddress.sin_family = AF_INET;
	_sockAddress.sin_addr.s_addr = inet_addr(m_sourceIP.String);
	_sockAddress.sin_port = htons(m_Sendport);

	//--------------------------------
	// Error Handling
	//--------------------------------
	if (connect(m_sSocket,(SOCKADDR*)&_sockAddress, sizeof(_sockAddress)) == SOCKET_ERROR)
	{
		m_lastError = WSAGetLastError();
		return;
	}
}

//######################################################################################
//######################################################################################
//						Thread Functions
//######################################################################################
//######################################################################################
///////////////////////////////////////////////////////////////////////////////
// Start Threads
///////////////////////////////////////////////////////////////////////////////
void NetBasement::startThreads()
{
	//--------------------------------
	// Create Listening Thread
	//--------------------------------
	m_tListen = CreateThread(
		NULL,                        // default security attributes 
		0,                           // use default stack size  
		ListenFunc,                  // thread function 
		this,					     // LPVOID param
		0,                           // use default creation flags 
		0);							 // returns the thread identifier 
	//--------------------------------
	// Error Handling
	//--------------------------------
	if(m_tListen == 0)
	{
		m_lastError = GetLastError();
		return;
	}
	//--------------------------------
	// Creat Sending Thread
	//--------------------------------
	m_tTalk = CreateThread(
		NULL,                        // default security attributes 
		0,                           // use default stack size  
		TalkFunc,			         // thread function 
		this,					     // LPVOID param
		0,                           // use default creation flags 
		0);							 // returns the thread identifier  (0 does nothing)
	//--------------------------------
	// Error Handling
	//--------------------------------
	if(m_tTalk == 0)
	{
		m_lastError = GetLastError();
		return;
	}
}
///////////////////////////////////////////////////////////////////////////////
// Listens and Dumps stuff onto box
///////////////////////////////////////////////////////////////////////////////
DWORD WINAPI ListenFunc(LPVOID lpParam)
{
	NetBasement* _NetPTR = (NetBasement*)lpParam;
	CircularBuffer* _inboxPTR = &(_NetPTR->m_inbox);
	while(1)
	{
		_inboxPTR->Write(_NetPTR->receiveFromSender());
	}
}
///////////////////////////////////////////////////////////////////////////////
// Sends stuff off the pile
///////////////////////////////////////////////////////////////////////////////
DWORD WINAPI TalkFunc(LPVOID lpParam)
{
	NetBasement* _NetPTR = (NetBasement*)lpParam;
	CircularBuffer* _outboxPTR = &(_NetPTR->m_outbox);
	Sleep(2000);
	while(1)
	{
		if(!(_outboxPTR->IsEmpty()))
			_NetPTR->sendMessage(_outboxPTR->ReadMsg());
	}
}

///////////////////////////////////////////////////////////////////////////////
// Get Inbox
///////////////////////////////////////////////////////////////////////////////
const CircularBuffer& NetBasement::getInbox() const
{
	return m_inbox;
}
//######################################################################################
//######################################################################################
//						Send/Receive Message
//######################################################################################
//######################################################################################
///////////////////////////////////////////////////////////////////////////////
// send data to receiver
///////////////////////////////////////////////////////////////////////////////
bool NetBasement::sendMessage(JWL_NET_MSG _msg)
{
	if(m_lastError != 0)	return false;					// Check if Errors Pending
	if(m_wsData == 0)		return false;					// Check if WinSock Active
	if(m_sSocket == 0)		return false;					// Check if Socket Created
	if(_msg.m_Size == 0)	return false;					// Check if Message is Longer than 0

	if(_msg.m_Size >= CB_MAX_MSG_SIZE)	return false;		// Check if Message is Longer than Max Message Size

	int _nBytes = 0;
	//-----------------------------------
	//	Send Message Size
	//-----------------------------------
	_nBytes = send(m_sSocket,(char*)&_msg.m_Size, sizeof(char), 0);

	//--------------------------------
	// Error Handling
	//--------------------------------
	if(_nBytes == SOCKET_ERROR)
	{
		m_lastError = WSAGetLastError();
		return false;
	}

	//-----------------------------------
	//	Send Message
	//-----------------------------------
	_nBytes = send(m_sSocket,_msg.m_Data, sizeof(char)*(_msg.m_Size), 0);

	//--------------------------------
	// Error Handling
	//--------------------------------
	if(_nBytes == SOCKET_ERROR)
	{
		m_lastError = WSAGetLastError();
		return false;
	}
	return true;
}


///////////////////////////////////////////////////////////////////////////////
// receive data from sender
///////////////////////////////////////////////////////////////////////////////
JWL_NET_MSG NetBasement::receiveFromSender()
{
	JWL_NET_MSG _null = {{0},0};
	if(m_lastError != 0)		return _null;					// Check if Errors Pending
	if(m_wsData == 0)			return _null;					// Check if WinSock Active
	if(m_rSocket == 0)			return _null;					// Check if Socket Created

	//-----------------------------------
	//	Set Local Vars
	//-----------------------------------
	int  _nBytes;
	JWL_NET_MSG _msg = {{0},0};

    //-----------------------------------
	//	Recieve Message Size
	//-----------------------------------
	_nBytes = recv(m_rSocket,(char*)&_msg.m_Size, sizeof(char), 0);

	//--------------------------------
	// Error Handling
	//--------------------------------
	if(_nBytes == SOCKET_ERROR)
	{
		m_lastError = WSAGetLastError();
		return _null;
	}

	if((_msg.m_Size == 0) || (_msg.m_Size >= CB_MAX_MSG_SIZE))
	{
		return _null;
	}

    //-----------------------------------
	//	Recieve Message
	//-----------------------------------
	_nBytes = recv(m_rSocket,(char*)&_msg.m_Data, sizeof(char)*_msg.m_Size, 0);

	//--------------------------------
	// Error Handling
	//--------------------------------
	if(_nBytes == SOCKET_ERROR)
	{
		m_lastError = WSAGetLastError();
		return _null;
	}
	return _msg;
}