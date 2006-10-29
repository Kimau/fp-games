#ifndef JWL_MESH_FUNCTION_COOL
#define JWL_MESH_FUNCTION_COOL

#include "CVector3.h"
#include <string.h>
typedef unsigned int UINT;

//----------------------------------
// Intermediate Model
// Data Structure
//----------------------------------
struct Intlvd_point
{
	float tx,ty;
	float nx,ny,nz;
	float vx,vy,vz;
};

class JWL_Mesh
{
public:
	JWL_Mesh(UINT _size, UINT _mode);
	~JWL_Mesh();

	void setP(UINT _offset, CTexCoord _tex, CVector3 _norm, CVector3 _vert);
	void setT(UINT _offset, double u, double v);
	void setTf(UINT _offset, float u, float v);
	void setT(UINT _offset, CTexCoord _tex);
	void setV(UINT _offset, double x, double y, double z);
	void setVf(UINT _offset, float x, float y, float z);
	void setV(UINT _offset, CVector3 _vert);
	void setN(UINT _offset, double x, double y, double z);
	void setN(UINT _offset, CVector3 _norm);
	void copyN(UINT _into, UINT _from);

	CVector3 getV(UINT _offset) const;
	CVector3 getN(UINT _offset) const;

	const UINT m_size;
	const UINT m_mode;
	Intlvd_point* m_array;

private:
	JWL_Mesh();
	JWL_Mesh(const JWL_Mesh& _ref);
};

#endif
