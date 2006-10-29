#include "JWLMesh.h"

//=======================================================
//	Constructor
//=======================================================
JWL_Mesh::JWL_Mesh(UINT _size, UINT _mode) : m_size(_size), m_mode(_mode)
{
	m_array = new Intlvd_point[m_size];		
	memset(m_array,0,sizeof(Intlvd_point)*m_size);
}

//=======================================================
//	Decontructor
//=======================================================
JWL_Mesh::~JWL_Mesh()
{
	delete[] m_array;
}

//=======================================================
//	Set Point with Double Vectors
//=======================================================
void JWL_Mesh::setP(UINT _offset, CTexCoord _tex, CVector3 _norm, CVector3 _vert)
{
	if(_offset < m_size)
	{
		m_array[_offset].tx = (float)_tex.u;			m_array[_offset].ty = (float)_tex.v;
		m_array[_offset].nx = (float)_norm.x;			m_array[_offset].ny = (float)_norm.y;			m_array[_offset].nz = (float)_norm.z;
		m_array[_offset].vx = (float)_vert.x;			m_array[_offset].vy = (float)_vert.y;			m_array[_offset].vz = (float)_vert.z;
	}
}

//=======================================================
//	Set Texture Point with Double
//=======================================================
void JWL_Mesh::setT(UINT _offset, double u, double v)
{
	if(_offset < m_size)
	{
		m_array[_offset].tx = (float)u;		
		m_array[_offset].ty = (float)v;
	}
}

//=======================================================
//	Set Texture Point with Float
//=======================================================
void JWL_Mesh::setTf(UINT _offset, float u, float v)
{
	if(_offset < m_size)
	{
		m_array[_offset].tx = u;		
		m_array[_offset].ty = v;
	}
}

//=======================================================
//	Set Texture Points with Double Vector
//=======================================================
void JWL_Mesh::setT(UINT _offset, CTexCoord _tex)
{
	if(_offset < m_size)
	{
		m_array[_offset].tx = (float)_tex.u;	
		m_array[_offset].ty = (float)_tex.v;
	}
}

//=======================================================
//	Set Vertex with double values
//=======================================================
void JWL_Mesh::setV(UINT _offset, double x, double y, double z)
{
	if(_offset < m_size)
	{
		m_array[_offset].vx = (float)x;		
		m_array[_offset].vy = (float)y;		
		m_array[_offset].vz = (float)z;
	}
}

//=======================================================
//	Set Vertex with float values
//=======================================================
void JWL_Mesh::setVf(UINT _offset, float x, float y, float z)
{
	if(_offset < m_size)
	{
		m_array[_offset].vx = x;		
		m_array[_offset].vy = y;		
		m_array[_offset].vz = z;
	}
}

//=======================================================
//	Set Vertex using double Vector
//=======================================================
void JWL_Mesh::setV(UINT _offset, CVector3 _vert)
{
	if(_offset < m_size)
	{
		m_array[_offset].vx = (float)_vert.x;
		m_array[_offset].vy = (float)_vert.y;	
		m_array[_offset].vz = (float)_vert.z;
	}
}

//=======================================================
//	Set Normal Using Double Values
//=======================================================
void JWL_Mesh::setN(UINT _offset, double x, double y, double z)
{
	if(_offset < m_size)
	{
		m_array[_offset].nx = (float)x;			
		m_array[_offset].ny = (float)y;		
		m_array[_offset].nz = (float)z;
	}
}

//=======================================================
//	Set Normal Double Vector
//=======================================================
void JWL_Mesh::setN(UINT _offset, CVector3 _norm)
{
	if(_offset < m_size)
	{
		m_array[_offset].nx = (float)_norm.x;
		m_array[_offset].ny = (float)_norm.y;	
		m_array[_offset].nz = (float)_norm.z;
	}
}

//=======================================================
//	Copy Normal from one point to another
//=======================================================
void JWL_Mesh::copyN(UINT _into, UINT _from)
{
	if((_into < m_size) && (_from < m_size))
	{
		m_array[_into].nx = m_array[_from].nx;
		m_array[_into].ny = m_array[_from].ny;
		m_array[_into].nz = m_array[_from].nz;
	}
}

//=======================================================
//	Get Vertex as Double Vector
//=======================================================
CVector3 JWL_Mesh::getV(UINT _offset) const
{
	if(_offset < m_size)
	{
		CVector3 _ret(m_array[_offset].vx, m_array[_offset].vy, m_array[_offset].vz);
		return _ret;			
	}
	return(CVector3(0,0,0));
}

//=======================================================
//	Get Normal as Double Vector
//=======================================================
CVector3 JWL_Mesh::getN(UINT _offset) const
{
	if(_offset < m_size)
	{
		CVector3 _ret(m_array[_offset].nx, m_array[_offset].ny, m_array[_offset].nz);
		return _ret;			
	}
	return(CVector3(0,0,0));
}