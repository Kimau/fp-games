#ifndef SURFACE_OF_HEIGHTANDNORMAL
#define SURFACE_OF_HEIGHTANDNORMAL

#include "CVector3.h"
typedef unsigned int UINT;

class CSurface
{
	//------------------------------------------
	//	Constructor
	//------------------------------------------
public:
	CSurface();
	CSurface(const CSurface& _ref);
	CSurface(UINT _width, UINT _length, UINT _maxHeight);
	CSurface& operator=(const CSurface& _ref);
	~CSurface();
	//------------------------------------------
	//	Functions
	//------------------------------------------
private:
	void GenerateHeights(UINT _maxHeight);
	void GenerateNormals();
	//------------------------------------------
	//	Accessors
	//------------------------------------------
public:
	float getHeight(UINT _x, UINT _y) const;
	const CVector3& getNormal(UINT _x, UINT _y, UINT _n = 0) const;
	const CVector3& getNormal(float _x, float _y) const;
	UINT  getWidth() const;
	UINT  getLength() const;
	//------------------------------------------
	//	Varibles and Accessors
	//------------------------------------------
private:
	UINT		m_width;
	UINT		m_length;
	float*		m_heights;
	CVector3*	m_normal;
};

#endif