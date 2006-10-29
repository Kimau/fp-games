#ifndef GOB_MATRIX_OBJECT_3454
#define GOB_MATRIX_OBJECT_3454
//----------------------------------
//	Includes
//----------------------------------
#include "GOB.h"
#include <vector>

//----------------------------------
//	Defines
//----------------------------------
#define GOBMATRIX_ID		1

#define GOBMATRIX_DIST		100
#define GOBMATRIX_WIDTH		GOBMATRIX_DIST*2
#define GOBMATRIX_LENGTH	GOBMATRIX_DIST*2
#define GOBMATRIX_POINTS	GOBMATRIX_WIDTH*GOBMATRIX_LENGTH

#define GOBMATRIX_SPEED		200.0f
#define GOBMATRIX_MAXSPEED	1.0f
#define GOBMATRIX_MAXHEIGHT 50.0f

//----------------------------------
//	Struct
//----------------------------------
struct MatrixPoint
{
	float m_height;
	float m_dir;
	float m_colour[3];
};

struct MatrixWeight
{
	int	  m_type;
	float m_size;
	DWORD m_age;
	DWORD m_life;
	float m_pos[3];
	float m_colour[3];
};

//------------------------------------------
//	GOB Matrix
//------------------------------------------
class GOBMatrix : public GOB
{
public:
	GOBMatrix();
	~GOBMatrix();

	void Render();
	void Update();
	int  Type();

	float getHeight(float _fx, float _fy) const;
	float getHeightFast(float _fx, float _fy) const;

	MatrixPoint m_plane[GOBMATRIX_WIDTH][GOBMATRIX_LENGTH];
	std::vector<MatrixWeight> m_weights;
};

#endif