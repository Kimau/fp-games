#ifndef __BASE_HH__
# define __BASE_HH__


# define SCREEN_W 1024
# define SCREEN_H 768

# define RENDER_LIMIT 4096


// Class pre-decs
class cMesh;
class cMat;


// Render states
enum eRenderState
{
	RENDER_SOLID=0,
	RENDER_BLENDED,
	RENDER_LOCKED,
	
	NUM_RENDER_STATES
};


// Syncroniser value
extern float gSync;


// Process class
class cProcess
{
private:
	// List pointers
	cProcess* mpPrev;
	cProcess* mpNext;
	
protected:
	// Mesh object, leave NULL if not drawable process
	cMesh* mpMesh;
	// Material object
	cMat* mpMat;
	
	// Kill process
	bool mKill;
	
public:
	// ctor / dtor
	cProcess();
	virtual ~cProcess();

	// Update func
	virtual void update();
	
	// Draw function
	void draw();
	// Submit function
	void submit();
	
	// Mesh accessor
	cMesh* getMesh(){ return mpMesh;}
	// Material accessor
	cMat* getMat(){ return mpMat;}
	
	// List accessors
	void setPrev(cProcess* lpProc){ mpPrev=lpProc;}
	cProcess* getPrev(){ return mpPrev;}
	void setNext(cProcess* lpProc){ mpNext=lpProc;}
	cProcess* getNext(){ return mpNext;}
	
	void kill(){ mKill=true;}
	bool killed(){ return mKill;}
};

// Kill all processes
void killProcs();

// Startup stuff
void init();
// Run game
void run();
// Stop game running
void stop();
// Shutdown stuff (hard stop)
void close();

// Random
int rnd(int lMin,int lMax);

// Get user input
void gatherInputs();
// Keyboard input
bool key(int lKey);
// Mouse input
int mX();
int mY();
int mZ();
int mVX();
int mVX();
bool msBtn(int lBtn);

// Sleep wrapper function, 'cause sdl functions are ugly
void sleep(int lMS);


#endif
