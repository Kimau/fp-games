#include "gMatrix.h"

//==================================================================
//	Create GOB Matrix
//==================================================================
GOBMatrix::GOBMatrix()
{
	memset(m_plane,0,sizeof(MatrixPoint) * GOBMATRIX_POINTS);

		MatrixWeight _weight = 
		{
			0,					// int	  m_type;
			5.0f,				// float m_size;
			0,					// DWORD m_age;
			0,					// DWORD m_life;
			{0.0f,0.0f,0.0f},	// float m_pos[3];
			{1.0f,0.0f,0.0f}	// float m_colour[3];
		};

		m_weights.push_back(_weight);
}
//==================================================================
//	Destroy
//==================================================================
GOBMatrix::~GOBMatrix()
{
}
//==================================================================
//	Render
//==================================================================
void GOBMatrix::Render()
{
	glPushMatrix();
		glTranslatef(m_Pos.x,m_Pos.y,m_Pos.z);

		//-----------------------------
		//	Black Under
		//-----------------------------
		glEnable(GL_DEPTH);
		for(int w = 0; w < (GOBMATRIX_WIDTH - 1); ++w)
		{
			float x = float(w - GOBMATRIX_DIST);
			glColor3f(0.0f,0.0f,0.0f);
			glBegin(GL_QUAD_STRIP);
			for(int l = 0; l < GOBMATRIX_LENGTH; ++l)
			{
				float y = float(l - GOBMATRIX_DIST);

				glVertex3f(x  ,y,m_plane[w  ][l].m_height - (x*x         + y*y) / 1000.0f - 0.01f);
				glVertex3f(x+1,y,m_plane[w+1][l].m_height - ((x+1)*(x+1) + y*y) / 1000.0f - 0.01f);
			}
			glEnd();
		}
		glDisable(GL_DEPTH);
		//-----------------------------
		//	Draw Lines
		//-----------------------------
		glLineWidth(0.2f);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);
		glDepthMask(false);
		//-----------------------------
		//	Horizontal Lines
		//-----------------------------
		for(int w = 0; w < GOBMATRIX_WIDTH; ++w)
		{
			float x = float(w - GOBMATRIX_DIST);
			glBegin(GL_LINE_STRIP);
			for(int l = 0; l < GOBMATRIX_LENGTH; ++l)
			{
				float y = float(l - GOBMATRIX_DIST);
				MatrixPoint& _point = m_plane[w][l];

				glColor3fv(_point.m_colour);
				glVertex3f(x,y, _point.m_height - (x*x + y*y) / 1000.0f );
			}
			glEnd();
		}

		//-----------------------------
		//	Vertical Lines
		//-----------------------------
		for(int l = 0; l < GOBMATRIX_LENGTH; ++l)
		{
			float y = float(l - GOBMATRIX_DIST);
			glBegin(GL_LINE_STRIP);

			for(int w = 0; w < GOBMATRIX_WIDTH; ++w)
			{
				float x = float(w - GOBMATRIX_DIST);
				MatrixPoint& _point = m_plane[w][l];

				glColor3fv(_point.m_colour);
				glVertex3f(x,y, _point.m_height - (x*x + y*y) / 1000.0f );
			}
			glEnd();
		}
		glDisable(GL_BLEND);
		glDepthMask(true);
		
	glPopMatrix();
}
//==================================================================
//	Update
//==================================================================
void GOBMatrix::Update()
{
	float _tran = float(x_delta) / GOBMATRIX_SPEED;
	//--------------------------------
	//	Loop Through Points
	//	Normalising
	//--------------------------------
	for(unsigned int w = 0; w < GOBMATRIX_WIDTH; ++w)
	{
		for(unsigned int l = 0; l < GOBMATRIX_LENGTH; ++l)
		{
			MatrixPoint& _point = m_plane[w][l];

			if(_point.m_dir < _point.m_height)
			{
				_point.m_height = _point.m_height * 0.9f + _point.m_dir * 0.1f;
			}
			else
			{
				_point.m_colour[0] *= 0.99f;
				_point.m_colour[1] = 0.5f * 0.01f + _point.m_colour[1] * 0.99f;
				_point.m_colour[2] *= 0.99f;

				_point.m_height = _point.m_height * 0.999f + _point.m_dir * 0.001f;
			}

			_point.m_dir = 0.0f;
		}
	}


	//--------------------------------
	//	Loop through Weights
	//--------------------------------
	for(int c = int(m_weights.size()) - 1; c >= 0; --c)
	{
		MatrixWeight& _weight = m_weights[c];
		_weight.m_age += x_delta;
		if((_weight.m_life > 0) && (_weight.m_age > _weight.m_life))
		{
			m_weights.erase(m_weights.begin() + c);
		}
		else
		{
			int _minX = min(GOBMATRIX_DIST-1,max(-GOBMATRIX_DIST,int(_weight.m_pos[0] - _weight.m_size)));
			int _maxX = min(GOBMATRIX_DIST-1,max(-GOBMATRIX_DIST,int(_weight.m_pos[0] + _weight.m_size)));
			for(int x = _minX; x <= _maxX; ++x)
			{
				int _minY = min(GOBMATRIX_DIST-1,max(-GOBMATRIX_DIST,int(_weight.m_pos[1] - _weight.m_size)));
				int _maxY = min(GOBMATRIX_DIST-1,max(-GOBMATRIX_DIST,int(_weight.m_pos[1] + _weight.m_size)));
				for(int y = _minY; y <= _maxY; ++y)
				{
					MatrixPoint& _point = m_plane[x + GOBMATRIX_DIST][y + GOBMATRIX_DIST];

					CVector2 _dir(_weight.m_pos[0] - x,_weight.m_pos[1] - y);
					float _inf = ((_weight.m_size*_weight.m_size) - (_dir.x*_dir.x + _dir.y*_dir.y)) / (_weight.m_size*_weight.m_size);

					if(_point.m_dir < 0)
					{
						_point.m_colour[0] += _weight.m_colour[0] * _inf;
						_point.m_colour[1] += _weight.m_colour[1] * _inf;
						_point.m_colour[2] += _weight.m_colour[2] * _inf;
					}
					else
					{
						_point.m_colour[0] = _weight.m_colour[0] * _inf;
						_point.m_colour[1] = _weight.m_colour[1] * _inf + 0.5f * (_weight.m_size - _inf);
						_point.m_colour[2] = _weight.m_colour[2] * _inf;
					}					

					_point.m_dir -= max(_weight.m_size * _inf / 4.0f, 0.0f);
				}
			}
		}
	}
}
//==================================================================
//	Update
//==================================================================
int GOBMatrix::Type()
{
	return GOBMATRIX_ID;
}

//==================================================================
//	Calculate Height
//==================================================================
float GOBMatrix::getHeight(float _fx, float _fy) const
{
	return getHeightFast(_fx,_fy);

	int w = int(_fx + GOBMATRIX_DIST);
	int l = int(_fy + GOBMATRIX_DIST);

	if((w < (GOBMATRIX_WIDTH - 1)) && (l < (GOBMATRIX_LENGTH - 1)))
	{
		return(
		(m_plane[w][l].m_height * (1.0f - (_fx - w)) + m_plane[w+1][l].m_height * (_fx - w) +
		 m_plane[w][l].m_height * (1.0f - (_fy - l)) + m_plane[w][l+1].m_height * (_fy - l)) / 2.0f);
	}
	else return 0.0f;
}
//==================================================================
//	Calculate Height FAST, inaccurate
//==================================================================
float GOBMatrix::getHeightFast(float _fx, float _fy) const
{
	int w = int(_fx + GOBMATRIX_DIST);
	int l = int(_fy + GOBMATRIX_DIST);

	if((w < GOBMATRIX_WIDTH) && (l < GOBMATRIX_LENGTH))
	{
		return m_plane[w][l].m_height;
	}
	else return 0.0f;
}
