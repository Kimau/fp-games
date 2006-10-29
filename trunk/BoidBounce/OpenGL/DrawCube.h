#ifndef THE_MIGHT_CUBED_DRAWER_HAHAHAHAHAHA_34543
#define THE_MIGHT_CUBED_DRAWER_HAHAHAHAHAHA_34543

//--------------------------------------
//	Header Includes
//--------------------------------------
#include "BaseGL.h"
#include "..\Math\CVector3.h"

class BGLDrawCube
{
	//----------------------------------------
	//	Construction
	//----------------------------------------
public:
	BGLDrawCube();
	BGLDrawCube(CVector3 _size);
	~BGLDrawCube();
	//----------------------------------------
	//	Functions
	//----------------------------------------
	void makeList();
	void Render();
	void setSize(CVector3 _size);
	void setInside(bool _inside);
	//----------------------------------------
	//	Accesors
	//----------------------------------------
	CVector3 getSize();
	//----------------------------------------
	//	Varibles
	//----------------------------------------
private:
	bool m_inside;		// Used to say whether the cube will be viewed from inside or outside
	GLuint m_list;
	CVector3 m_size;
};

#endif