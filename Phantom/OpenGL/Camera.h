#ifndef CAMERA_CLASS_FOR_BASIC_OPEN_GL_STRUCTURE
#define CAMERA_CLASS_FOR_BASIC_OPEN_GL_STRUCTURE

//-----------------------------------
//	Headers
//-----------------------------------
#include "BaseGL.h"
#include "..\Math\CVector3.h"

class BGLCamera
{
	//-------------------------------
	//	Constructors
	//-------------------------------
public:
	BGLCamera();
	~BGLCamera();

	//-------------------------------
	//	Functions
	//-------------------------------
public:
	void LookAt();
	void Rotate(float _rotation);
	void Rotate(float _rotation, CVector3 _axis);
	void Rotate(float _rotation, CVector3 _axis, CVector3 _point);

	//-------------------------------
	//	Set Functions
	//-------------------------------
	void setSubject(CVector3 _subject);
	void setEye(CVector3 _eye);
	void setUp(CVector3 _up);
	void set(CVector3 _subject, CVector3 _eye, CVector3 _up);

	//-------------------------------
	//	Accessors
	//-------------------------------
public:
	CVector3 getSubject() const;
	CVector3 getEye() const;
	CVector3 getUp() const;

	//-------------------------------
	//	Varibles
	//-------------------------------
private:
	CVector3 m_Subject;
	CVector3 m_Eye;
	CVector3 m_Up;
};

#endif