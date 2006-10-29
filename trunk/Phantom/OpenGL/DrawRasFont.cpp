#include "DrawRasFont.h"
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//			Initialisation
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//=========================================================================================
// BLANK creation.... NEVER to be used excepted by scope
//=========================================================================================
BGLRasterFont::BGLRasterFont()
: m_id(0),
  m_engine(0),
  m_height(0)
{
}
//=========================================================================================
// Creates Raster Font
//=========================================================================================
void BGLRasterFont::Load(WinGLengine* _engine, int _height, const char* _font)
{
	if(m_id)
		glDeleteLists(m_id,1);

	m_engine = _engine;
	m_height = _height;

	HFONT	hFont;										// This will store the handle to our font

	m_id = glGenLists(BGL_MAX_CHAR);				// Generate the list for the font

	hFont = CreateFont(	_height,						// Our desired HEIGHT of the font
						0,								// The WIDTH (If we leave this zero it will pick the best width depending on the height)
						0,								// The angle of escapement
						0,								// The angle of orientation
						FW_BOLD,						// The font's weight (We want it bold)
						FALSE,							// Italic - We don't want italic
						FALSE,							// Underline - We don't want it underlined
						FALSE,							// Strikeout - We don't want it strikethrough
						ANSI_CHARSET,					// This is the type of character set
						OUT_TT_PRECIS,					// The Output Precision
						CLIP_DEFAULT_PRECIS,			// The Clipping Precision
						ANTIALIASED_QUALITY,			// The quality of the font - We want anitaliased fonts
						FF_DONTCARE|DEFAULT_PITCH,		// The family and pitch of the font.  We don't care.
						_font);							// The font name (Like "Arial", "Courier", etc...)

	wglUseFontBitmaps(m_engine->getDeviceCon(), 0, BGL_MAX_CHAR - 1, m_id);	// Builds 255 bitmap characters
}
//=========================================================================================
//	Copy Constructor
//=========================================================================================
BGLRasterFont::BGLRasterFont(const BGLRasterFont& _ref)
: m_id(_ref.m_id),
  m_engine(_ref.m_engine),
  m_height(_ref.m_height)
{
}
//=========================================================================================
//	Copy Constructor (Equals Operator)
//=========================================================================================
BGLRasterFont& BGLRasterFont::operator=(const BGLRasterFont& _ref)
{
	if(this != &_ref)
	{
		m_id = _ref.m_id;
		m_height = _ref.m_height;
		m_engine = _ref.m_engine;
	}
	return *this;
}
//=========================================================================================
//	Delete the Display List
//=========================================================================================
BGLRasterFont::~BGLRasterFont()
{
	if(m_id)
		glDeleteLists(m_id,1);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//			Render Functions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//=========================================================================================
//	Draw Font
//=========================================================================================
void BGLRasterFont::Render(float x, float y, const char* _format, ...)
{
	//---------------------------------
	//  Generate Text
	//---------------------------------
	if(_format == NULL)		return;

	char _input[BGL_MAX_CHAR] = {0};
	va_list _argPtr;

	va_start(_argPtr, _format);		
	vsprintf(_input, _format, _argPtr);	
	va_end(_argPtr);

	//---------------------------------
	//  Disable Wanted Flags
	//---------------------------------
	glPushAttrib(GL_LIGHTING_BIT | GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DITHER);

		//---------------------------------
		//  Position Text Cursor
		//---------------------------------
		glPushAttrib( GL_TRANSFORM_BIT | GL_VIEWPORT_BIT );
			// Here we use a new projection and modelview matrix to work with.
			glMatrixMode( GL_PROJECTION );
			glPushMatrix();
				glLoadIdentity();
				glMatrixMode( GL_MODELVIEW );
				glPushMatrix();
					glLoadIdentity();

					//----------------------------------------------
					// Calculate the weird screen position
					//----------------------------------------------
					y = m_engine->getWinHeight() - 35 - m_height - y;				

					glViewport(int(x) - 1, int(y) - 1, 0, 0 );					// Create a new viewport to draw into
					glRasterPos4f( 0, 0, 0, 1 );						// Set the drawing position

					glPopMatrix();
				glMatrixMode( GL_PROJECTION );
			glPopMatrix();
		glPopAttrib();

		//---------------------------------
		// Call Lists
		//---------------------------------
		glPushMatrix();
			glColor3f(1.0f,1.0f,1.0f);
			glListBase(m_id);
			glCallLists((int)strlen(_input), GL_UNSIGNED_BYTE, _input);
		glPopMatrix();	

	//---------------------------------
	//  Renable Flags
	//---------------------------------
	glPopAttrib();
}

//=========================================================================================
//	Draw Block of Text
//=========================================================================================
void BGLRasterFont::RenderBlock(float x, float y, char* _block)
{
	//---------------------------------
	//  Generate Text
	//---------------------------------
	if(_block == NULL)		return;

	char* _tok = 0;
	//---------------------------------
	//  Disable Wanted Flags
	//---------------------------------
	glPushAttrib(GL_LIGHTING_BIT | GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DITHER);

		_tok = strtok(_block,"\n");
		while(_tok)
		{
			//---------------------------------
			//  Position Text Cursor
			//---------------------------------
			glPushAttrib( GL_TRANSFORM_BIT | GL_VIEWPORT_BIT );
				// Here we use a new projection and modelview matrix to work with.
				glMatrixMode( GL_PROJECTION );
				glPushMatrix();
					glLoadIdentity();
					glMatrixMode( GL_MODELVIEW );
					glPushMatrix();
						glLoadIdentity();

						//----------------------------------------------
						// Calculate the weird screen position
						//----------------------------------------------
						int _tempY = m_engine->getWinHeight() - 35 - m_height - int(y);				

						glViewport( int(x) - 1, _tempY - 1, 0, 0 );				// Create a new viewport to draw into
						glRasterPos4f( 0, 0, 0, 1 );						// Set the drawing position

						glPopMatrix();
					glMatrixMode( GL_PROJECTION );
				glPopMatrix();
			glPopAttrib();

			//---------------------------------
			// Call Lists
			//---------------------------------
			glPushMatrix();
				glColor3f(1.0f,1.0f,1.0f);
				glListBase(m_id);
				glCallLists((int)strlen(_tok), GL_UNSIGNED_BYTE, _tok);
			glPopMatrix();

			//---------------------------------
			//	Iterate
			//---------------------------------
			y += m_height + 8 ;
			_tok = strtok(0,"\n");
		}

	//---------------------------------
	//  Renable Flags
	//---------------------------------
	glPopAttrib();
}