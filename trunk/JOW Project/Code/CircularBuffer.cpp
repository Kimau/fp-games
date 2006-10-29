#include "CircularBuffer.h"
//######################################################################################
//######################################################################################
//						Construct / Deconstruct
//######################################################################################
//######################################################################################
///////////////////////////////////////////////////////////////////////////////
/// constructor.
// owner: is the instantiator the owner or not?
//////////////////////////////////////////////////////////////////////////////
CircularBuffer::CircularBuffer(bool _owner) : m_Parent(0)
{
	if (_owner)
	{
		//--------------------------------------------
		//	Set Data Block
		//--------------------------------------------
		m_data = (char*)malloc(CB_BUFSIZE);
		memset(m_data , 0, sizeof(char)*CB_BUFSIZE);

		m_Ready = true;
	}
	else
	{
		m_Ready = false;
		m_data = 0;
	}
	m_readOffset = 0;
	m_writeOffset = 0;
}
///////////////////////////////////////////////////////////////////////////////
// Dconstructor
///////////////////////////////////////////////////////////////////////////////
CircularBuffer::~CircularBuffer()
{
	if(m_data)
		free(m_data);
}

///////////////////////////////////////////////////////////////////////////////
// copy constructor.
///////////////////////////////////////////////////////////////////////////////
CircularBuffer::CircularBuffer(const CircularBuffer& _circBuff) : m_Parent(&_circBuff)
{
	m_readOffset = 0;
	m_writeOffset = 0;

	m_Ready = false;
	m_data = 0;
}

//######################################################################################
//######################################################################################
//						Read / Write
//######################################################################################
//######################################################################################
///////////////////////////////////////////////////////////////////////////////
// Read a Packet off Circ Buffer
///////////////////////////////////////////////////////////////////////////////
OurPacket* CircularBuffer::ReadPacket() const
{
	//-------------------------------
	//	Get Head
	//-------------------------------
	UINT _msgCount;
	IP_ADDRESS_STRING _ip = {0};
	{
		JWL_NET_MSG _msg = ReadMsg();
		
		if((_msg.m_Size == 20) && (_msg.m_Data[0] == 'P') && (_msg.m_Data[1] == 'B') && (_msg.m_Data[2] == '!'))
		{
			_msgCount = (int)_msg.m_Data[3];
			memcpy(_ip.String,&(_msg.m_Data[4]),sizeof(char)*16);
		}
		else return 0;
	}
	//-------------------------------
	//	Get Body
	//-------------------------------
	char* _body = (char*)malloc(sizeof(char)*(_msgCount*CB_BUFSIZE));
	UINT _offset = 0;

	for(UINT c = 0; c < _msgCount; ++c)
	{
		JWL_NET_MSG _msg = ReadMsg();
		memcpy(&(_body[_offset]),_msg.m_Data,sizeof(char)*_msg.m_Size);
		_offset += _msg.m_Size;
	}
	//-------------------------------
	//	Get Foot
	//-------------------------------
	{
		JWL_NET_MSG _msg = ReadMsg();
		
		if((_msg.m_Size == 3) && (_msg.m_Data[0] == 'P') && (_msg.m_Data[1] == 'E') && (_msg.m_Data[2] == '!'))
		{
			OurPacket* _ret = new OurPacket(_offset,_ip,_body);
			free(_body);
			return _ret;
		}
		else
		{
			free(_body);
			return 0;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
// Read from circular buffer.
///////////////////////////////////////////////////////////////////////////////
JWL_NET_MSG CircularBuffer::ReadMsg() const
{
	//----------------------------
	// Parent reads to Child
	//----------------------------
	if(m_Parent)
	{
		return m_Parent->ReadMsg();
	}
	//----------------------------
	// Is buffer ready for Read
	//----------------------------
	if(!m_Ready)
	{
		JWL_NET_MSG _null = {{0},0};
		return _null;
	}
	//----------------------------
	// Read from Buffer
	//----------------------------
	JWL_NET_MSG _msg = {{0},0};
	UINT _size = 0;
	while(_size < CB_MAX_MSG_SIZE)
	{
		//----------------------------------
		// Saftey Checks
		//----------------------------------

		if(m_readOffset >= CB_BUFSIZE)		*(const_cast<UINT*>(&m_readOffset)) = 0;
		if(m_readOffset == m_writeOffset)	break;

        //----------------------------------
		// Read Data
		//----------------------------------		
		_msg.m_Data[_size++] = m_data[m_readOffset];
		*(const_cast<UINT*>(&m_readOffset)) = m_readOffset + 1;
	}
	//----------------------------------
	// Retrn Message
	//----------------------------------
	_msg.m_Size = _size;
	return _msg;
}
///////////////////////////////////////////////////////////////////////////////
// Write PACKET to circular buffer.
///////////////////////////////////////////////////////////////////////////////
bool CircularBuffer::Write(OurPacket _packet)
{
	if(m_Parent)
		return false;

    //--------------------------------
	// Construct and Send HEAD
	// + "PB!"
	// + [1]  Message Count
	// + [16] IP
	//--------------------------------
	UINT _msgCount = (_packet.getSize() / CB_MAX_MSG_SIZE) + ((_packet.getSize() % CB_MAX_MSG_SIZE) != 0);
	{
		char _head[CB_MAX_MSG_SIZE] = {'P','B','!',(char)_msgCount};

		memcpy(&(_head[4]),(_packet.getIP()).String,sizeof(char)*16);

		if (!Write(_head,20))	return false;
	}
	//--------------------------------
	// Send Body
	//--------------------------------
	UINT _offset = 0;
	for(UINT c = 0; c < _msgCount; ++c)
	{
		if(c == (_msgCount - 1)) // Last Packet
		{
			UINT _size = (_packet.getSize() % CB_MAX_MSG_SIZE);
			JWL_NET_MSG _msg = {{0},_size};
			memcpy(_msg.m_Data,&((_packet.getData())[_offset]),_size);
			if (!Write(_msg))	return false;
		}
		else
		{
			JWL_NET_MSG _msg = {{0},CB_MAX_MSG_SIZE};
			memcpy(_msg.m_Data,&((_packet.getData())[_offset]),CB_MAX_MSG_SIZE);
			if (!Write(_msg))	return false;
		}
	}
	//--------------------------------
	//	Send Foot
	//--------------------------------
	{
		return Write("PE!",3);
	}
}
///////////////////////////////////////////////////////////////////////////////
// Write to circular buffer.
///////////////////////////////////////////////////////////////////////////////
bool CircularBuffer::Write(JWL_NET_MSG _msg)
{
	return Write(_msg.m_Data, _msg.m_Size);
}

///////////////////////////////////////////////////////////////////////////////
// Write to circular buffer.
///////////////////////////////////////////////////////////////////////////////
bool CircularBuffer::Write(char* _Data, UINT _Size)
{
	//----------------------------
	// Children Not allowed to write
	// _Msg Size Check
	//----------------------------
	if((m_Parent) || (_Size >= CB_MAX_MSG_SIZE))
		return false;
	m_Ready = false;

	//----------------------------
	// Check if Message Will
	// overwrite
	//----------------------------
	UINT _finalOff = m_writeOffset + _Size;
	if(_finalOff < CB_BUFSIZE)
	{
		//------------------------------------
		// Simply Logic - work it out
		//------------------------------------
		if((m_writeOffset < m_readOffset) != (_finalOff < m_readOffset))
		{
			m_Ready = true;
			return false;
		}
		//----------------------------
		// Write Data and Iterate
		//----------------------------
		memcpy(&(m_data[m_writeOffset]),_Data,sizeof(char)*_Size);
		m_writeOffset += _Size;

		m_Ready = true;
		return true;
	}
	else
	{
		//------------------------------------
		// Simply Logic - work it out
		//------------------------------------
		if((m_readOffset < _Size))
		{
			m_Ready = true;
			return false;
		}
		//----------------------------
		// Write Data and Iterate
		//----------------------------
		memcpy(&m_data,_Data,sizeof(char)*_Size);
		m_writeOffset = _Size;

		m_Ready = true;
		return true;
	}
}

//######################################################################################
//######################################################################################
//						Accesors
//######################################################################################
//######################################################################################
///////////////////////////////////////////////////////////////////////////////
// is circ buff ready for I/O?
///////////////////////////////////////////////////////////////////////////////
bool CircularBuffer::IsReady() const
{
	if(m_Parent == 0) 
		return m_Ready;
	else
		return m_Parent->IsReady();
}

///////////////////////////////////////////////////////////////////////////////
// is instantiator the owner?
///////////////////////////////////////////////////////////////////////////////
bool CircularBuffer::IsOwner() const
{
	return (m_Parent == 0);
}
bool CircularBuffer::IsEmpty() const
{
	if(m_Parent == 0)
		return(m_readOffset == m_writeOffset);
	else
		return(m_Parent->IsEmpty());
}