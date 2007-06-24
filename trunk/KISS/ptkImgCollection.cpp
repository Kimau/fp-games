#include "ptkImgCollection.h"
#include <map>

//-------------------------------
//	Comparision Class
//-------------------------------
struct ltstr
{
	bool operator()(const char* s1, const char* s2) const
	{
		return stricmp(s1, s2) < 0;
	}
};

typedef std::map<const char*, pIC_Image, ltstr> ImgMap;
ImgMap s_Images;	// Image Map

//################################
//################################
//	Image Collection Class
//################################
//################################
//===============================================================================
//	Start Image Collection
//===============================================================================
pImageCollection::pImageCollection()
{

}
//===============================================================================
//	Clean up Image Collection
//===============================================================================
pImageCollection::~pImageCollection()
{
}
//===============================================================================
//	Clean up Image Collection
//===============================================================================
void pImageCollection::Purge()
{
	//---------------------------
	//	Free Memory
	//---------------------------
	for(ImgMap::iterator _Iter = s_Images.begin(); _Iter != s_Images.end(); ++_Iter)
	{
		if(_Iter->second.m_Image != 0)
		{
			delete _Iter->second.m_Image;
		}
	}

	s_Images.clear();
}

//===============================================================================
//	Is the Collection Empty
//===============================================================================
bool pImageCollection::Empty()
{
	return s_Images.empty();
}

//===============================================================================
//	Fetch Image by Filename
//===============================================================================
KGraphic* pImageCollection::operator[](const char* _filename)
{
	return s_Images[_filename].m_Image;
}

//===============================================================================
//	Assigns an Image
//===============================================================================
KGraphic* pImageCollection::Assign(const char* _filename)
{
	//-----------------------------------
	//	Check if Image is already loaded
	//-----------------------------------
	if(s_Images[_filename].m_Image == 0)
	{
		char* _lwrFile = new char[strlen(_filename) + 1];
		strcpy(_lwrFile,_filename);
		_lwrFile = strlwr(_lwrFile);
		FILE* _file = fopen(_filename, "r");

		//-----------------------------------
		//	If File Exsists
		//-----------------------------------
		if(_file != 0)
		{
			fclose(_file);
			//-----------------------------------
			//	Load Graphic
			//-----------------------------------
			pIC_Image _temp;
			_temp.m_Image = KPTK::createKGraphic();
			bool _ret = _temp.m_Image->loadPicture(_lwrFile, true, true);

			//-----------------------------------
			//	Insert Filename
			//-----------------------------------
			if(_ret)
			{
				_temp.m_Count = 1;
				s_Images[_filename] = _temp;
				return  _temp.m_Image;
			}
		}
	}
	//-----------------------------------
	//	Image is Already Loaded
	//	Increment Dependent
	//-----------------------------------
	else
	{
		s_Images[_filename].m_Count += 1;
		return s_Images[_filename].m_Image;
	}
	return 0;
}
//===============================================================================
//	Assign an Image with Colour Key
//===============================================================================
KGraphic* pImageCollection::AssignKey(const char* _filename, unsigned int r, unsigned int g, unsigned int b)
{
	//-----------------------------------
	//	Check if Image is already loaded
	//-----------------------------------
	if(s_Images[_filename].m_Image == 0)
	{
		char* _lwrFile = new char[strlen(_filename) + 1];
		strcpy(_lwrFile,_filename);
		_lwrFile = strlwr(_lwrFile);
		FILE* _file = fopen(_filename, "r");

		//-----------------------------------
		//	If File Exsists
		//-----------------------------------
		if(_file != 0)
		{
			fclose(_file);
			//-----------------------------------
			//	Load Graphic
			//-----------------------------------
			pIC_Image _temp;
			_temp.m_Image = KPTK::createKGraphic();
			_temp.m_Image->setColorKey(true,r,g,b);
			bool _ret = _temp.m_Image->loadPicture(_lwrFile, true, true);

			//-----------------------------------
			//	Insert Filename
			//-----------------------------------
			if(_ret)
			{
				_temp.m_Count = 1;
				s_Images[_filename] = _temp;
				return  _temp.m_Image;
			}
		}
	}
	//-----------------------------------
	//	Image is Already Loaded
	//	Increment Dependent
	//-----------------------------------
	else
	{
		s_Images[_filename].m_Count += 1;
		return s_Images[_filename].m_Image;
	}
	return 0;
}
//===============================================================================
//	Releases an image and deletes if free
//===============================================================================
void pImageCollection::Release(const char* _filename)
{
	if(s_Images[_filename].m_Image != 0)
	{
		pIC_Image _temp = s_Images[_filename];
		_temp.m_Count -= 1;

		//------------------------------
		//	If not used anymore Delete
		//------------------------------
		if(_temp.m_Count < 1)
			Erase(_filename);
	}
}
//===============================================================================
//	Delete an item regardless
//===============================================================================
void pImageCollection::Erase(const char* _filename)
{
	if(s_Images[_filename].m_Image != 0)
	{
		KGraphic* _img = s_Images[_filename].m_Image;
		if(_img == 0)
		{
			delete _img;
		}

		s_Images.erase(_filename);
	}
}
