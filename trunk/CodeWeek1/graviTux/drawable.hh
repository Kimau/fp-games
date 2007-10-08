#ifndef __DRAWABLE_HH__
# define __DRAWABLE_HH__


// Class pre-decs
class cMesh;
class cMat;


// Alpha component generation
enum eAlphaTypes
{
	// Alpha generation
	ALPHA_NO_CHANGE=0,
	ALPHA_ZERO,
	ALPHA_FULL,
	ALPHA_VALUE,
	ALPHA_COL_KEY,
};


// Init OpenGL
void initGL();
// Setup the frustum
void initFrustum();
// Flush the screen
void flushScreen();


// Camera functions
void camSetPos(float lX,float lY,float lDist);
float camGetX();
float camGetY();
float camGetDist();

// Apply camera translation
void camTranslate();
void camUntranslate();


// Get pick vector
void getPickVec(cVec3& lVec,int lX,int lY);


// Set colour key
void setColKey(char lR,char lG,char lB);
// Set clear colour
void setClearColour(float lR,float lG,float lB);


// Make a flat plane object (helper function)
cMesh* makePlane(float lW,float lH);
// Load a texture (helper function)
cMat* loadTexture(const char* lpFile,eAlphaTypes lAlpha,cMat* lpMask);


// Vertex struct
struct sVertex
{
	cVec3 mPos;
	cVec2 mUV;
	sVertex():mPos(),mUV(){}
	sVertex(cVec3 lPos,cVec2 lUV):mPos(lPos),mUV(lUV){}
};


// Mesh class
class cMesh
{
private:
	// Vertex array
	sVertex* mpVtxs;
	int mVtxCount;
	// Index array
	int* mpInds;
	int mIndCurr,mIndCount;
	
	// Vertex stride
	int mVtxStride;
	
	// Translation matrix
	cMtx16 mTrans;
	
	// Radius
	float mRadius;
	
	// Distance from screen
	float mDist;

	// Base colour (should be in material, but this is more fun)
	cFCol mCol;
	
	// Locked to screen, visible
	bool mLocked,mVisible;
	
public:
	cMesh();
	~cMesh();
	
	// Add vertex
	void addVtx(cVec3 lPos,cVec2 lUV);
	void addVtx(sVertex lVtx);
		
	// Make mesh arrays
	void make(int lVtxs);
	// Clean out array data
	void clean();
	
	// Render
	void draw();
	// Check object is on-screen
	bool onScreen();
	// Get radix value
	inline unsigned char getRadix(int lRadix){ return ((unsigned char*)&mDist)[lRadix];}
	
	// Vertex array accessors
	sVertex* getVtxs(){ return mpVtxs;}
	int getVtxCount(){ return mVtxCount;}
	// Index array accessors
	int* getInds(){ return mpInds;}
	int getIndCount(){ return mIndCount;}
	void setIndCurr(int lCount){ mIndCurr=lCount;}
	int getIndCurr(){ return mIndCurr;}
	
	// Set position
	void setPos(cVec3 lPos);
	void setPos(float lX,float lY,float lZ);
	
	// Rotate
	void zRotate(float lAng);

	// Colour
	void setColR(float lCol){ mCol.R=lCol;}
	void setColG(float lCol){ mCol.G=lCol;}
	void setColB(float lCol){ mCol.B=lCol;}
	void setColA(float lCol){ mCol.A=lCol;}
	void setCol(cFCol lCol){ mCol=lCol;}
	
	// Screen locking accessors
	void setLocked(bool lLock){ mLocked=lLock;}
	bool getLocked(){ return mLocked;}
	
	// Visibility accessors
	void setVisible(bool lVisible){ mVisible=lVisible;}
	bool getVisible(){ return mVisible;}
};


// Material class
class cMat
{
private:
	// Texture data
	char* mpData;
	// Size (square)
	int mSize;
	// Texture object
	int mTObj;
	
	// Blending
	int mBlendSrc,mBlendDst;
	bool mBlended;
	
	// Set alpha component
	void setAlpha(eAlphaTypes lAlpha,cMat* lpMask);

public:
	cMat();
	~cMat();

	// Load texture from file
	void load(const char* lpFile,eAlphaTypes lAlpha,cMat* lpMask);
	// Clean out data
	void clean();
	
	// Rendering
	void setGLStates();

	// Texture data accessor
	char* getTexData(){ return mpData;}
	// Size accessor
	int getTexSize(){ return mSize;}
	
	// Blending
	void solid();
	void ghost();
	void mask();
	bool getBlended(){ return mBlended;}
};


#endif
