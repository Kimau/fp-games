#include "OurPacket.h"


//######################################################################################
//######################################################################################
//						Construct / Deconstruct
//######################################################################################
//######################################################################################
OurPacket::OurPacket()
{
	IP_ADDRESS_STRING _null = {{0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0}};
	OurPacket(0,_null,0);
}

OurPacket::OurPacket(UINT _dataSize, IP_ADDRESS_STRING _sourceIP, char* _data) :
	m_sourceIP(_sourceIP),
	m_dataSize(_dataSize)
{
	m_dataBlock = (char*)malloc(sizeof(char) * _dataSize);
	memcpy(m_dataBlock, _data, (sizeof(char) * _dataSize));
}
//-------------------------------------------------------------------
//	Free Up Memory
//-------------------------------------------------------------------
OurPacket::~OurPacket()
{
	if(m_dataBlock)
		free(m_dataBlock);
}

//######################################################################################
//######################################################################################
//						ACCESSORS
//######################################################################################
//######################################################################################
///////////////////////////////////////////////////////////////////////////////
// Extracts the data from the packet.
///////////////////////////////////////////////////////////////////////////////
const char* OurPacket::getData() const
{
	return m_dataBlock;
}

///////////////////////////////////////////////////////////////////////////////
// Extracts IP from packet.
///////////////////////////////////////////////////////////////////////////////
const IP_ADDRESS_STRING OurPacket::getIP() const
{
	return m_sourceIP;
}


///////////////////////////////////////////////////////////////////////////////
// Extracts random message size
///////////////////////////////////////////////////////////////////////////////
unsigned long OurPacket::getSize() const
{
	return m_dataSize;
}