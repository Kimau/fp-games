#ifndef Q_CAMERA_CLASS_3245
#define Q_CAMERA_CLASS_3245

//-----------------------------------
//	Headers
//-----------------------------------
#include "BaseGL.h"
#include "..\Math\CQuaternion.h"
#include "..\Math\CVector3.h"

//-----------------------------------------------------
//	An Animated Quaterion Camera
//-----------------------------------------------------
class QCamera
{
public:
	//-------------------------------------
	//	Construction and Set
	//-------------------------------------
	QCamera();
	~QCamera();

	//-------------------------------------
	//	Base Functions
	//-------------------------------------
	void LookAt();
	void UpdatePer(DWORD _delta);
	void UpdateUnit(DWORD _delta);
	//-------------------------------------
	//	Set Functions
	//-------------------------------------	
	void setCamera   (float _px, float _py, float _pz,
					  float _ex, float _ey, float _ez,
					  float _ux, float _uy, float _uz, bool _anim = false);
	void setPosition (float _px, float _py, float _pz, bool _anim = false);
	void setEyeVector(float _ex, float _ey, float _ez, bool _anim = false);
	void setUpVector (float _ux, float _uy, float _uz, bool _anim = false);

	void RotatePos(float _deg, CVector3 _axis, bool _anim = false);
	void RotateEye(float _deg, CVector3 _axis, bool _anim = false);
	void RotateUp (float _deg, CVector3 _axis, bool _anim = false);

	void Zoom(float _step, bool _anim = false);
	void Pan(float _step, bool _anim = false);

	//-------------------------------------
	//	Accessors
	//-------------------------------------
	CVector3 getView() const;
	CVector3 getLatitude() const;
	//-------------------------------------	
	//	Varibles
	//-------------------------------------	
	float m_current[9];		// Current - Pos[3],  Eye[3], Up[3]
	float m_target[9];		// Target  - Pos[3],  Eye[3], Up[3]
	float m_spPercent;		// Camera Animation Speed % per 100 ticks (1.0 means animation will complete in 1 tick)
	float m_spUnit;			// Camera Animation Speed per Units
	float m_progress;		// Morph Progress between Old and Target
};

#endif