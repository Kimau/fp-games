#ifndef BASIC_CG_PROFILE_FOR_BGL_465436
#define BASIC_CG_PROFILE_FOR_BGL_465436

//----------------------------------
// HEader Includes
//----------------------------------
#include "BaseGL.h"
#include <Cg\cg.h>
#include <Cg\cgGL.h>
#include <vector>

//----------------------------------
//	Libary Links
//----------------------------------
#pragma comment(lib, "cg.lib")
#pragma comment(lib, "cggl.lib")

//---------------------------------
//	Basic CG Class
//---------------------------------
class BaseCG
{
public:
	//---------------------------
	//	Construction
	//---------------------------
	BaseCG();
	~BaseCG();

	//---------------------------
	//	Load Program
	//---------------------------
	void Start();
	int LoadProg(const char* _filename, const char* _entry, const char** _args, bool _frag);
	void Bind(int _id, bool _frag);
	void Unbind(bool _frag);

	//---------------------------
	//	Varibles
	//---------------------------
private:
	bool m_Ready;				// Did CG Load

	CGcontext m_Context;				// CG Context
	CGprofile m_vProfile;				// CG Vertex Profile
	CGprofile m_fProfile;				// CG Fragment Profile
	
	std::vector<CGprogram> m_Shader;	// CG Program	
};

#endif