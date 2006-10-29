#include "BaseCG.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//		CONSTRUCTION
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//========================================================================
//	Basic CG Constructor
//========================================================================
BaseCG::BaseCG()
{
	//---------------------------------------------------
	//	Init CG
	//---------------------------------------------------
	m_vProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
	m_fProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
	if(	(m_vProfile == CG_PROFILE_UNKNOWN) || 
		(m_vProfile == CG_INVALID_ENUMERANT_ERROR) || 
		(m_fProfile == CG_PROFILE_UNKNOWN) || 
		(m_fProfile == CG_INVALID_ENUMERANT_ERROR))
		m_Ready = false;
	else
		m_Ready = true;

	m_Context = cgCreateContext();
}
//========================================================================
//	DeInit
//========================================================================
BaseCG::~BaseCG()
{
	for(int c = 0; c < int(m_Shader.size()); ++c)
		cgDestroyProgram(m_Shader[c]);
	
	cgDestroyContext(m_Context);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//		FUNCTIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//========================================================================
//	Load Program
//========================================================================
int BaseCG::LoadProg(const char* _filename, const char* _entry, const char** _args, bool _frag)
{
	//----------------------------------------
	//	Load Program
	//----------------------------------------
	CGprogram _ret = 0;
	if(_frag)
		_ret = cgCreateProgramFromFile(m_Context, CG_SOURCE, _filename, m_fProfile, _entry, _args);
	else
		_ret = cgCreateProgramFromFile(m_Context, CG_SOURCE, _filename, m_vProfile, _entry, _args);

	//----------------------------------------
	//	IF program Loader
	//----------------------------------------
	if(_ret)
	{
		size_t _id = m_Shader.size();
		m_Shader.push_back(_ret);	
		cgGLLoadProgram(m_Shader[_id]);
		return int(_id);
	}
	else
		return -1;	
}
//========================================================================
//	Run Program
//========================================================================
void BaseCG::Bind(int _id, bool _frag)
{
	if((_id >= 0) && (_id < int(m_Shader.size())))
	{
		CGparameter _ModelProj = cgGetNamedParameter(m_Shader[_id], "modelViewProj");
		cgGLSetStateMatrixParameter(_ModelProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

		cgGLBindProgram(m_Shader[_id]);

		if(_frag)
			cgGLEnableProfile(m_fProfile);
		else
			cgGLEnableProfile(m_vProfile);
	}
}
//========================================================================
//	Stop Program
//========================================================================
void BaseCG::Unbind(bool _frag)
{
	if(_frag)
	{
		cgGLUnbindProgram(m_fProfile);		
		cgGLDisableProfile(m_fProfile);
	}
	else
	{
		cgGLUnbindProgram(m_vProfile);		
		cgGLDisableProfile(m_vProfile);
	}	
}