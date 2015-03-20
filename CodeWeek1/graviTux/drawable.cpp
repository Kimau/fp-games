// Include system headers
#include <math.h>
#include <fstream>
#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/gl.h>

// Include game headers
#include "./types.hh"
#include "./base.hh"
#include "./drawable.hh"


// Colour key
cICol gColKey;


// Frustum array
float gFrustum[6][4];
// Pick modifier
cVec3 gPickMod;


// Camera data
cVec3 gCamPos;


// Last material to be set
cMat* gpLastMat=0x0;


// Init OpenGL
void initGL()
{
	glClearDepth(1.0f);
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	setClearColour(0.0f,0.0f,0.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT,GL_FILL);
	glPolygonMode(GL_BACK,GL_LINE);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
	initFrustum();
	glLoadIdentity();
}


// Setup the frustum
void initFrustum()
{
	float lN=1.0f,lF=200.0f,lL,lR,lB,lT,lZoom=0.5f,lRatio=(float)SCREEN_H/(float)SCREEN_W;

	lL=-1.0f*lZoom; lR=1.0f*lZoom;
	lB=-lRatio*lZoom; lT=lRatio*lZoom;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Make frustum
	cMtx16 lMtx;
	glFrustum(lL,lR,lB,lT,lN,lF);
	glGetFloatv(GL_PROJECTION_MATRIX,(GLfloat*)&lMtx);
	
	// Get pick modifiers
	gPickMod.X=lR*2.0f;
	gPickMod.Y=lT*2.0f;
	gPickMod.Z=-lN;

//	mXMod=1.0f/mViewMtx.i[0];
//	mYMod=1.0f/mViewMtx.i[5];
//	mZMod=1.0f/mViewMtx.i[5];
	
	// Frustum planes
	gFrustum[0][0]=lMtx.i[3]+lMtx.i[0]; gFrustum[0][1]=lMtx.i[7]+lMtx.i[4];
	gFrustum[0][2]=lMtx.i[11]+lMtx.i[8]; gFrustum[0][3]=lMtx.i[15]+lMtx.i[12];
	gFrustum[1][0]=lMtx.i[3]-lMtx.i[0]; gFrustum[1][1]=lMtx.i[7]-lMtx.i[4];
	gFrustum[1][2]=lMtx.i[11]-lMtx.i[8]; gFrustum[1][3]=lMtx.i[15]-lMtx.i[12];
	gFrustum[2][0]=lMtx.i[3]+lMtx.i[1]; gFrustum[2][1]=lMtx.i[7]+lMtx.i[5];
	gFrustum[2][2]=lMtx.i[11]+lMtx.i[9]; gFrustum[2][3]=lMtx.i[15]+lMtx.i[13];
	gFrustum[3][0]=lMtx.i[3]-lMtx.i[1]; gFrustum[3][1]=lMtx.i[7]-lMtx.i[5];
	gFrustum[3][2]=lMtx.i[11]-lMtx.i[9]; gFrustum[3][3]=lMtx.i[15]-lMtx.i[13];
	gFrustum[4][0]=lMtx.i[3]+lMtx.i[2]; gFrustum[4][1]=lMtx.i[7]+lMtx.i[6];
	gFrustum[4][2]=lMtx.i[11]+lMtx.i[10]; gFrustum[4][3]=lMtx.i[15]+lMtx.i[14];
	gFrustum[5][0]=lMtx.i[3]-lMtx.i[2]; gFrustum[5][1]=lMtx.i[7]-lMtx.i[6];
	gFrustum[5][2]=lMtx.i[11]-lMtx.i[10]; gFrustum[5][3]=lMtx.i[15]-lMtx.i[14];
	
	// Normalise frustum
	float lMag;
	int t;
	for(t=0; t<6; ++t)
	{
		lMag=1.0f/sqrt((gFrustum[t][0]*gFrustum[t][0])+
			(gFrustum[t][1]*gFrustum[t][1])+(gFrustum[t][2]*gFrustum[t][2]));
		gFrustum[t][0]*=lMag; gFrustum[t][1]*=lMag; gFrustum[t][2]*=lMag; gFrustum[t][3]*=lMag;
	}

	glMatrixMode(GL_MODELVIEW);
}


// Flush the screen
void flushScreen()
{
	SDL_GL_SwapBuffers();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}


// Set camera position
void camSetPos(float lX,float lY,float lDist){ gCamPos=cVec3(-lX,-lY,-lDist);}
// Get camera X value
float camGetX(){ return -gCamPos.X;}
// Get camera Y value
float camGetY(){ return -gCamPos.Y;}
// Get camera Dist value
float camGetDist(){ return -gCamPos.Z;}


// Apply camera translation
void camTranslate()
{
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(gCamPos.X,gCamPos.Y,gCamPos.Z);
}


// Close camera translation
void camUntranslate()
{
	glPopMatrix();
}


// Get pick vector
void getPickVec(cVec3& lVec,int lX,int lY)
{
	lVec.X=((lX/(float)SCREEN_W)-0.5f)*gPickMod.X;
	lVec.Y=(0.5f-(lY/(float)SCREEN_H))*gPickMod.Y;
	lVec.Z=gPickMod.Z;
}


// Set colour key
void setColKey(char lR,char lG,char lB)
{
	gColKey=cICol(lR,lG,lB);
}


// Set clear colour
void setClearColour(float lR,float lG,float lB)
{
	glClearColor(lR,lG,lB,1.0f);
}


// Make a flat plane object (helper function)
cMesh* makePlane(float lW,float lH)
{
	lW*=0.5f;
	lH*=0.5f;
	// Create mesh and fill vertex array
	cMesh* lpMesh=new cMesh;
	lpMesh->make(6);
	lpMesh->addVtx(cVec3(-lW,lH,0),cVec2(0,1));
	lpMesh->addVtx(cVec3(-lW,-lH,0),cVec2(0,0));
	lpMesh->addVtx(cVec3(lW,-lH,0),cVec2(1,0));
	lpMesh->addVtx(cVec3(-lW,lH,0),cVec2(0,1));
	lpMesh->addVtx(cVec3(lW,-lH,0),cVec2(1,0));
	lpMesh->addVtx(cVec3(lW,lH,0),cVec2(1,1));
	return lpMesh;
}


// Load a texture (helper function)
cMat* loadTexture(const char* lpFile,eAlphaTypes lAlpha,cMat* lpMask)
{
	cMat* lpMat=new cMat;
	lpMat->load(lpFile,lAlpha,lpMask);
	return lpMat;
}


// Mesh constructor
cMesh::cMesh():
	mpVtxs(0x0),mVtxCount(0),mpInds(0x0),mIndCurr(0),mIndCount(0),mRadius(0.0f),
	mCol(1,1,1,1),mLocked(false),mVisible(true)
{
	mVtxStride=sizeof(sVertex);
	mTrans.ident();
}


// Mesh destructor
cMesh::~cMesh()
{
	clean();
}


// Add vertex
void cMesh::addVtx(cVec3 lPos,cVec2 lUV)
{
	addVtx(sVertex(lPos,lUV));
}


// Add vertex
void cMesh::addVtx(sVertex lVtx)
{
	if (mIndCurr>=mIndCount) return;
	// Insert data
	mpInds[mIndCurr]=mIndCurr;
	mpVtxs[mIndCurr]=lVtx;
	// Check new radius
	float lRad=lVtx.mPos^lVtx.mPos;
	if (lRad>mRadius) mRadius=lRad;
	// Inc index cursor
	++mIndCurr;
}


// Make mesh arrays
void cMesh::make(int lVtxs)
{
	clean();
	mpVtxs=new sVertex[lVtxs];
	mpInds=new int[lVtxs];
	// Don't moan about the pointlessness of this, I might need separate arrays later
	mVtxCount=lVtxs;
	mIndCount=lVtxs;
	mIndCurr=0;
	mRadius=0.0f;
}


// Clean out array data
void cMesh::clean()
{
	if (mpVtxs) delete []mpVtxs;
	if (mpInds) delete []mpInds;
	mVtxCount=0;
	mIndCount=0;
	mIndCurr=0;
	mRadius=0.0f;
}


// Render the mesh object
void cMesh::draw()
{
	if (!mVisible) return;
	
	// Set colour
	glColor4f(mCol.R,mCol.G,mCol.B,mCol.A);
	
	glPushMatrix();
	glMultMatrixf((GLfloat*)&mTrans);
	
	glVertexPointer(3,GL_FLOAT,mVtxStride,&mpVtxs[0].mPos);
	glTexCoordPointer(2,GL_FLOAT,mVtxStride,(GLvoid*)(&mpVtxs[0].mUV));
	glDrawElements(GL_TRIANGLES,mIndCurr,GL_UNSIGNED_INT,mpInds);
	
	glPopMatrix();
}


// Check object is on-screen
bool cMesh::onScreen()
{
	int t;
	cVec3 lPos=cVec3(mTrans.i[12],mTrans.i[13],mTrans.i[14]);
	float lVZ=lPos.Z-gCamPos.Z,lDist;
	mDist=lVZ;
	if (lVZ<0) lVZ*=-lVZ; else lVZ*=lVZ;
	if (lVZ<-mRadius) return false;
	// Perform frustum check
	if (!mLocked){ lPos.X+=gCamPos.X; lPos.Y+=gCamPos.Y; lPos.Z+=gCamPos.Z;}
	for(t=0; t<6; ++t)
	{
		lDist=(gFrustum[t][0]*lPos.X)+(gFrustum[t][1]*lPos.Y)+(gFrustum[t][2]*lPos.Z)+gFrustum[t][3];
		if ((lDist<0)&&(lDist*lDist>mRadius)) return false;
	}
	return true;
}


// Rotate mesh
void cMesh::zRotate(float lAng)
{
	// Extract angular components
	float lS=sinf(lAng),lC=cosf(lAng);
	mTrans.i[0]=lC;
	mTrans.i[1]=-lS;
	mTrans.i[4]=lS;
	mTrans.i[5]=lC;
}


// Set position
void cMesh::setPos(cVec3 lPos)
{
	mTrans.i[12]=lPos.X; mTrans.i[13]=lPos.Y; mTrans.i[14]=lPos.Z;
}


// Set position
void cMesh::setPos(float lX,float lY,float lZ)
{
	mTrans.i[12]=lX; mTrans.i[13]=lY; mTrans.i[14]=lZ;
}


// Material constructor
cMat::cMat():
	mpData(0x0),mSize(0),mTObj(0)
{
	solid();
}


// Material destructor
cMat::~cMat()
{
	clean();
}


// Set alpha components
void cMat::setAlpha(eAlphaTypes lAlpha,cMat* lpMask)
{
	if (lAlpha==ALPHA_NO_CHANGE) return;
	// If using a mask, check sizes are the same
	if (lpMask&&(lpMask->getTexSize()!=mSize)){ printf("Texture mask size mismatch\n"); return;}
	// Get source tex data
	cICol* lpSrc=(cICol*)mpData;
	if (lpMask) lpSrc=(cICol*)lpMask->getTexData();
	// Step through each pixel and adjust alpha component based on source tex
	int t,lSize=mSize*mSize;
	cICol lPix;
	for(t=0; t<lSize; ++t)
	{
		lPix=lpSrc[t];
		if (lAlpha==ALPHA_ZERO) lPix.A=0;
		else if (lAlpha==ALPHA_FULL) lPix.A=255;
		else if (lAlpha==ALPHA_VALUE) lPix.A=lPix.val();
		else if (lAlpha==ALPHA_COL_KEY){ gColKey.A=lPix.A; lPix.A=(lPix!=gColKey)*255;}
		((cICol*)mpData)[t].A=lPix.A;
	}
}


// Load texture from file
void cMat::load(const char* lpFile,eAlphaTypes lAlpha,cMat* lpMask)
{
	// Clean out old data
	clean();
	
	// Try to load the file
	std::ifstream lFile;
	lFile.open(lpFile,std::ios::binary);
	if (!lFile.is_open()){ printf("Could not load file %s\n",lpFile); return;}
	// Load texture data -----
	// Read and check BM flag
	char lBM[2];
	lFile.read(lBM,2);
	if ((lBM[0]!='B')||(lBM[1]!='M')){ printf("File %s is not recognised as BMP format\n",lpFile); lFile.close(); return;}
	// Get data offset
	int lOffset;
	lFile.seekg(10,std::ios::beg);
	lFile.read((char*)&lOffset,4);
	// Read width+height (check square and put into mSize)
	int lWidth,lHeight;
	lFile.seekg(18,std::ios::beg);
	lFile.read((char*)&lWidth,4);
	lFile.read((char*)&lHeight,4);
	if (lWidth!=lHeight){ printf("Bitmap %s is not square\n",lpFile); lFile.close(); return;}
	mSize=lWidth;
	// Read bits depth
	char lBPP,lStep;
	lFile.seekg(28,std::ios::beg);
	lFile.read(&lBPP,1);
	if (lBPP==24) lStep=3;
	else if (lBPP==32) lStep=4;
	else{ printf("Bitdepth of %s not supported\n",lpFile); lFile.close(); return;}
	// Read data
	lFile.seekg(lOffset,std::ios::beg);
	mpData=new char[mSize*mSize*4];
	int t,tn;
	for(t=0; t<mSize*mSize; ++t)
		for(tn=lStep-1; tn>=0; --tn) lFile.read((char*)mpData+((t*4)+tn),1);
	// Finished with file
	lFile.close();
	
	// Set alpha components
	setAlpha(lAlpha,lpMask);
	
	// Create GL tex object
	glGenTextures(1,(GLuint*)&mTObj);
	glBindTexture(GL_TEXTURE_2D,mTObj);
	// Submit data
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,mSize,mSize,0,GL_RGBA,GL_UNSIGNED_BYTE,mpData);
	// Set filters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	// Bind null texture
	glBindTexture(GL_TEXTURE_2D,0);
}


// Clean out data
void cMat::clean()
{
	mSize=0;
	if (!mpData) return;
	// Clean out GL tex object
	glDeleteTextures(1,(GLuint*)&mTObj);
	mTObj=0;
	// Clean out data
	delete []mpData;
}


// Set GL states for rendering
void cMat::setGLStates()
{
	// Early out
	if (gpLastMat==this) return;
	
	// Bind the texture object
	glBindTexture(GL_TEXTURE_2D,mTObj);

	// Set blending
	if (mBlended)
	{
		glBlendFunc(mBlendSrc,mBlendDst);
		glEnable(GL_BLEND);
	}
	else glDisable(GL_BLEND);
	
	gpLastMat=this;
}


// Set blend mode to solid (default)
void cMat::solid()
{
	mBlended=false;
}


// Set blend mode to ghosted
void cMat::ghost()
{
	mBlendSrc=GL_SRC_ALPHA;
	mBlendDst=GL_ONE;
	mBlended=true;
}


// Set blend mode to masked
void cMat::mask()
{
	mBlendSrc=GL_SRC_ALPHA;
	mBlendDst=GL_ONE_MINUS_SRC_ALPHA;
	mBlended=true;
}
