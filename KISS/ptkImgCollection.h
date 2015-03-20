#ifndef PTK_IMAGE_COLLECTION_CLASS_564212
#define PTK_IMAGE_COLLECTION_CLASS_564212

#include "ptk.h"

//-------------------------------
//	Image Struct
//-------------------------------
struct pIC_Image
{
	KGraphic* m_Image;
	int m_Count;
};

//-------------------------------
//	Image Collection Class
//-------------------------------
class pImageCollection
{
public:
	pImageCollection();
	~pImageCollection();
	void Purge();

	bool Empty();										// Is the Collection Empty

	KGraphic* operator[](const char* _filename);		// Fetch Image by Filename

	KGraphic* Assign(const char* _filename);			// Assigns an Image
	KGraphic* AssignKey(const char* _filename, 
		unsigned int r, unsigned int g, unsigned int b);// Assigns an Image
	void Release(const char* _filename);				// Releases an image and deletes if free
	void Erase(const char* _filename);					// Delete an item regardless
};

#endif