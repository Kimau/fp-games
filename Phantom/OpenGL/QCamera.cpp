#include "QCamera.h"

//####################################
//####################################
//	Construction and Set
//####################################
//####################################
//=======================================================================
//	Create Blank Camera
//=======================================================================
QCamera::QCamera() : m_spPercent(0.1f), m_spUnit(1.0f), m_progress(0.0f)
{
	memset(m_current,0,sizeof(float)*9);
	m_current[15] = 1.0f;
	memset(m_target, 0,sizeof(float)*9);
}
//=======================================================================
//	Deconstructor
//=======================================================================
QCamera::~QCamera()
{
}

//####################################
//####################################
//	Base Functions
//####################################
//####################################
//=======================================================================
//	Update OpenGL to Camera
//=======================================================================
void QCamera::LookAt()
{
	gluLookAt(	m_current[0],m_current[1],m_current[2],
				m_current[3],m_current[4],m_current[5],
				m_current[6],m_current[7],m_current[8]);
}
//=======================================================================
//	Update Movement Based on Percentage 
//=======================================================================
void QCamera::UpdatePer(DWORD _delta)
{
	if(m_progress >= 1.0f)
		return;

	for(int c = 0; c < 9; ++c)
	{
		float _factor = max(m_spPercent * float(_delta) / 100.0f, 0.5f);
		m_current[c] = m_current[c] * (1.0f - _factor) + m_target[c] * _factor;
	}
}

//=======================================================================
//	Update Movement Based on Units
//=======================================================================
void QCamera::UpdateUnit(DWORD _delta)
{
	//---------------------------------
	//	Check if Animation is ON
	//---------------------------------
	if(m_progress >= 1.0f)
		return;
	bool _completed = true;

	for(int c = 0; c < 8; c += 3)
	{
		CVector3 _dir(m_target[c] - m_current[c], m_target[c+1] - m_current[c+1], m_target[c+2] - m_current[c+2]);
		//---------------------------------
		//	Check the is Animation to Do
		//---------------------------------
		float _sqrDist = _dir.SqrdMag();
		if(_sqrDist < 0.01f)
			continue;
		else
			_completed = false;

		//---------------------------------
		//	Scale Length
		//---------------------------------		
		if(_sqrDist > 1.0f)
			_dir.Normalize();

		_dir *= m_spUnit * float(_delta) / 100.0f;

		//---------------------------------
		//	Update Position
		//---------------------------------
		m_current[c] += _dir.x;
		m_current[c+1] += _dir.y;
		m_current[c+2] += _dir.z;
	}

	if(_completed)
		m_progress = 1.1f;
}
//####################################
//####################################
//	Set Functions
//####################################
//####################################
//=======================================================================
//	Set Camera
//=======================================================================
void QCamera::setCamera (float _px, float _py, float _pz,
						 float _ex, float _ey, float _ez,
						 float _ux, float _uy, float _uz, bool _anim)
{
	if(_anim)
	{
		// Set Position
		m_target[0] = _px;
		m_target[1] = _py;
		m_target[2] = _pz;

		// Set Eye
		m_target[3] = _ex;
		m_target[4] = _ey;
		m_target[5] = _ez;

		// Set Up
		m_target[6] = _ux;
		m_target[7] = _uy;
		m_target[8] = _uz;

		m_progress = 0.0f;
	}
	else
	{
		// Set Position
		m_current[0] = _px;
		m_current[1] = _py;
		m_current[2] = _pz;

		// Set Eye
		m_current[3] = _ex;
		m_current[4] = _ey;
		m_current[5] = _ez;

		// Set Up
		m_current[6] = _ux;
		m_current[7] = _uy;
		m_current[8] = _uz;
	}
}
//=======================================================================
//	Set Position
//=======================================================================
void QCamera::setPosition(float _px, float _py, float _pz, bool _anim)
{
	if(_anim)
	{
		m_target[0] = _px;
		m_target[1] = _py;
		m_target[2] = _pz;
		m_progress = 0.0f;
	}
	else
	{
		m_current[0] = _px;
		m_current[1] = _py;
		m_current[2] = _pz;
	}
}
//=======================================================================
//	Set Eye
//=======================================================================
void QCamera::setEyeVector(float _ex, float _ey, float _ez, bool _anim)
{
	if(_anim)
	{
		m_target[3] = _ex;
		m_target[4] = _ey;
		m_target[5] = _ez;
		m_progress = 0.0f;
	}
	else
	{
		m_current[3] = _ex;
		m_current[4] = _ey;
		m_current[5] = _ez;
	}
}
//=======================================================================
//	Set Up
//=======================================================================
void QCamera::setUpVector(float _ux, float _uy, float _uz, bool _anim)
{
	if(_anim)
	{
		m_target[6] = _ux;
		m_target[7] = _uy;
		m_target[8] = _uz;
		m_progress = 0.0f;
	}
	else
	{
		m_current[6] = _ux;
		m_current[7] = _uy;
		m_current[8] = _uz;
	}
}

//=======================================================================
//	Rotate Camera Position around Eye Target
//=======================================================================
void QCamera::RotatePos(float _deg, CVector3 _axis, bool _anim)
{
	CVector3 _vec(	m_current[0] - m_current[3],
					m_current[1] - m_current[4],
					m_current[2] - m_current[5]);

	_vec.Rotate(_deg,_axis);

	if(_anim)
	{
		memcpy(m_target,m_current,sizeof(float)*9);

		m_target[0] = m_target[3] + _vec.x;
		m_target[1] = m_target[4] + _vec.y;
		m_target[2] = m_target[5] + _vec.z;

		m_progress = 0.0f;
	}
	else
	{
		m_current[0] = m_current[3] + _vec.x;
		m_current[1] = m_current[4] + _vec.y;
		m_current[2] = m_current[5] + _vec.z;
	}
}
//=======================================================================
//	Rotate Camera Eye Target around Position
//=======================================================================
void QCamera::RotateEye(float _deg, CVector3 _axis, bool _anim)
{
	CVector3 _vec(	m_current[3] - m_current[0],
					m_current[4] - m_current[1],
					m_current[5] - m_current[2]);

	_vec.Rotate(_deg,_axis);

	if(_anim)
	{
		memcpy(m_target,m_current,sizeof(float)*9);

		m_target[3] = m_target[0] + _vec.x;
		m_target[4] = m_target[1] + _vec.y;
		m_target[5] = m_target[2] + _vec.z;

		m_progress = 0.0f;
	}
	else
	{
		m_current[3] = m_current[0] + _vec.x;
		m_current[4] = m_current[1] + _vec.y;
		m_current[5] = m_current[2] + _vec.z;
	}
}
//=======================================================================
//	Rotate Camera Up Vector
//=======================================================================
void QCamera::RotateUp(float _deg, CVector3 _axis, bool _anim)
{
	CVector3 _vec(m_current[6],m_current[7],m_current[8]);

	_vec.Rotate(_deg,_axis);

	if(_anim)
	{
		memcpy(m_target,m_current,sizeof(float)*9);

		m_target[6] = _vec.x;
		m_target[7] = _vec.y;
		m_target[8] = _vec.z;

		m_progress = 0.0f;
	}
	else
	{
		m_current[6] = _vec.x;
		m_current[7] = _vec.y;
		m_current[8] = _vec.z;
	}
}
//=======================================================================
//	Move Towards Focus
//=======================================================================
void QCamera::Zoom(float _step, bool _anim)
{
	//---------------------------------
	//	Get View Vector
	//---------------------------------
	CVector3 _vec(	m_current[0] - m_current[3],
					m_current[1] - m_current[4],
					m_current[2] - m_current[5]);

	if(_vec.SqrdMag() < 1.0f)
	{
		_vec.Normalize();
	}
	_vec *= _step;

	//---------------------------------
	//	Adjusts Vector
	//---------------------------------
	if(_anim)
	{
		memcpy(m_target,m_current,sizeof(float)*9);

		m_target[0] -= _vec.x;
		m_target[1] -= _vec.y;
		m_target[2] -= _vec.z;

		m_progress = 0.0f;
	}
	else
	{
		m_current[0] -= _vec.x;
		m_current[1] -= _vec.y;
		m_current[2] -= _vec.z;
	}
}
//=======================================================================
//	Pan Camera
//=======================================================================
void QCamera::Pan(float _step, bool _anim)
{
	//---------------------------------
	//	Get View Vector
	//---------------------------------
	CVector3 _vec(	m_current[0] - m_current[3],
					m_current[1] - m_current[4],
					m_current[2] - m_current[5]);

	_vec = _vec.getLatitude();
	if(_vec.SqrdMag() < 1.0f)
	{
		_vec.Normalize();
	}
	_vec *= _step;

	//---------------------------------
	//	Adjusts Vector
	//---------------------------------
	if(_anim)
	{
		memcpy(m_target,m_current,sizeof(float)*9);

		m_target[0] += _vec.x;
		m_target[1] += _vec.y;
		m_target[2] += _vec.z;

		m_progress = 0.0f;
	}
	else
	{
		m_current[0] += _vec.x;
		m_current[1] += _vec.y;
		m_current[2] += _vec.z;
	}
}
//=======================================================================
//	Get Lattitude of View Vector
//=======================================================================
CVector3 QCamera::getView() const
{
	CVector3 _vec(	m_current[0] - m_current[3],
					m_current[1] - m_current[4],
					m_current[2] - m_current[5]);

	return _vec;
}
//=======================================================================
//	Get Lattitude of View Vector
//=======================================================================
CVector3 QCamera::getLatitude() const
{
	CVector3 _vec(	m_current[0] - m_current[3],
					m_current[1] - m_current[4],
					m_current[2] - m_current[5]);

	return _vec.getLatitude();
}