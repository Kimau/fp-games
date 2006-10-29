#include "Light.h"

//###########################
//###########################
//		CONSTRUTION
//###########################
//###########################
//============================================================================
//	Construciton
//============================================================================
BGLLights::BGLLights()
{
}
//============================================================================
//	Destruction
//============================================================================
BGLLights::~BGLLights()
{
	while(m_lights.empty() != 0)
	{
		glDisable(m_lights[0].m_ID);
		m_lights.erase(m_lights.begin());
	}
}

//###########################
//###########################
//		LIGHT MANAGEMENT
//###########################
//###########################
//============================================================================
//	Add Light
//============================================================================
int BGLLights::AddLight()
{
	int _lightID = GL_LIGHT0;
	bool _flag = true;
	while(_flag)
	{
		//----------------------
		//	See if ID is found
		//----------------------
		for(int c = int(m_lights.size() - 1); c >= 0; --c)
		{
			_flag &= (m_lights[c].m_ID != _lightID);
		}

		//----------------------
		//	NOT FOUND
		//----------------------
		if(_flag)
		{
			BGLLightStruct _light = { _lightID,				
				{0,0,0,1},			//  m_ambient[4];
				{1,1,1,1},			//  m_diffuse[4];
				{1,1,1,1},			//  m_specular[4];
				{5.0f,5.0f,9.0f,1},	//  m_position[4];

				0,					//  m_constant_attenuation;
				0,					//  m_linear_attenuation;
				0.02f,				//  m_quadratic_attenuation;

				{0,0,-1},			//  m_spot_direction[3];
				0,					//  m_spot_exponent;
				180};				//  m_spot_cutoff;
			
			m_lights.push_back(_light);
			update(int(m_lights.size()) - 1);
			return _lightID;
		}
		//----------------------
		//	MATCH
		//----------------------
		else
		{
			++_lightID;
			_flag = true;
		}
	}
	return 0;
}
//============================================================================
//	Delete Light
//============================================================================
void BGLLights::DelLight(int _lightID)
{
	glDisable(m_lights[_lightID].m_ID);
	m_lights.erase(m_lights.begin());
}
//============================================================================
//	Update Light
//============================================================================
void BGLLights::update(int _index)
{
	//-----------------------------
	//	Update Light
	//-----------------------------
	int _lightID = m_lights[_index].m_ID;
	BGLLightStruct& _light = m_lights[_index];
	glLightfv(_lightID, GL_AMBIENT,		_light.m_ambient);
	glLightfv(_lightID, GL_DIFFUSE,		_light.m_diffuse);
	glLightfv(_lightID, GL_SPECULAR,	_light.m_specular);
	glLightfv(_lightID, GL_POSITION,	_light.m_position);

	glLightf(_lightID, GL_CONSTANT_ATTENUATION,		_light.m_constant_attenuation);
	glLightf(_lightID, GL_LINEAR_ATTENUATION,		_light.m_linear_attenuation);
	glLightf(_lightID, GL_QUADRATIC_ATTENUATION,	_light.m_quadratic_attenuation);

	glLightfv(_lightID, GL_SPOT_DIRECTION,	_light.m_spot_direction);
	glLightf(_lightID, GL_SPOT_EXPONENT,	_light.m_spot_exponent);
	glLightf(_lightID, GL_SPOT_CUTOFF,		_light.m_spot_cutoff);

	//-----------------------------
	// Enable Light
	//-----------------------------
	glEnable(_light.m_ID);
}
