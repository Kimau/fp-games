#ifndef BASIC_GL_LIGHT_34326
#define BASIC_GL_LIGHT_34326

//-----------------------------------
//	Headers
//-----------------------------------
#include "BaseGL.h"
#include <vector>

struct BGLLightStruct
{
	unsigned int m_ID;
	float m_ambient[4];
	float m_diffuse[4];
	float m_specular[4];
	float m_position[4];

	float m_constant_attenuation;
	float m_linear_attenuation;
	float m_quadratic_attenuation;

	float m_spot_direction[3];
	float m_spot_exponent;
	float m_spot_cutoff;
};

//-----------------------------------
//	Basic Light Class
//-----------------------------------
class BGLLights
{
public:
	//-------------------------
	//	Constructor
	//-------------------------
	BGLLights();
	~BGLLights();

	//-------------------------
	//	Light Management
	//-------------------------
	int AddLight();
	void DelLight(int _lightID);
	void update(int _lightID);

	//-------------------------
	//	Vars
	//-------------------------
	std::vector<BGLLightStruct> m_lights;
};

#endif