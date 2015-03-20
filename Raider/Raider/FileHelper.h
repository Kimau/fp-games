#ifndef RAIDER_HEADER_FILE_HELPER
#define RAIDER_HEADER_FILE_HELPER

/*----------------------------------
	 Structs
 ----------------------------------*/
struct cpBody;
struct cpShape;
struct cpSpace;
struct cpVect;
struct gpMultiBody
{
	int m_vCount;
	cpVect* m_Verts;
	float m_Radius;
	float m_Mass;
	float m_Friction;
	float m_Elastic;
	float m_Inertia;
	unsigned int m_Colgroup;
};


void SaveSpace(cpSpace* _space, const char* _filename);
cpSpace* LoadSpace(const char* _filename );

void addStaticSegment(cpVect _start, cpVect _end, cpSpace* _space);
void addStaticCircle(float _radius, cpVect _offset, cpSpace* _space);
void addStaticPoly(int _vCount, cpVect* _verts, cpSpace* _space);

cpShape* addPoly(gpMultiBody& _poly, cpVect _pos, cpSpace* _space);
cpBody* addPlayer(cpSpace* _space);
void addItem(cpSpace* _space, cpVect _pos);

#endif