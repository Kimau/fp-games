#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <string.h>
#include <malloc.h>
#include "Structs.h"
#include "OurPacket.h"


///////////////////////////////////////////////////////////////////////////////
// A fine collection of stuff needed to implement the circular buffer, sir.
///////////////////////////////////////////////////////////////////////////////
class CircularBuffer
{
	//---------------------------
	// Con and Decon
	//---------------------------
private:
	CircularBuffer();
public:
	CircularBuffer(bool _owner);
	~CircularBuffer();
	CircularBuffer(const CircularBuffer& _circbuff);

	//---------------------------
	// Read and Write
	//---------------------------
	OurPacket*	ReadPacket() const;
	JWL_NET_MSG ReadMsg() const;
	bool Write(OurPacket _packet);
	bool Write(JWL_NET_MSG _msg);
	bool Write(char* _Data, UINT _Size);

	//---------------------------
	// Flag Checks
	//---------------------------
	bool IsReady() const;
	bool IsOwner() const;
	bool IsEmpty() const;

private:
	//------------------
	// Offsets
	//------------------
	UINT m_readOffset;
	UINT m_writeOffset;

	//------------------
	// Flags
	//------------------
	bool m_Ready;
	//------------------
	// Data
	//------------------
	char* m_data;							// pointer to Chars
	const CircularBuffer* m_Parent;			// Constant pointer to Constant Value
};
#endif