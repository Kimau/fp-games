#ifndef OURPACKET_H
#define OURPACKET_H

#include <stdlib.h>
#include <string.h>
#include "Structs.h"

// ****************************************************************************
// A class that describes what our packet looks like.													 
//*****************************************************************************
class OurPacket
{
public:
		OurPacket();
		OurPacket(UINT _dataSize, IP_ADDRESS_STRING _sourceIP, char* _data);
		~OurPacket();

		//---------------------------------------
		// Accesors
		//---------------------------------------
		const char* getData() const;
		const IP_ADDRESS_STRING getIP() const;
		unsigned long getSize() const;
		
private:
	//---------------------------------------
	// Header
	//---------------------------------------
	IP_ADDRESS_STRING	m_sourceIP;
	UINT				m_dataSize;
    
	// Data Block
	char*				m_dataBlock;
};

#endif