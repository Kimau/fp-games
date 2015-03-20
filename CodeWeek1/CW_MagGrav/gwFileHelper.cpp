#include "gwFileHelper.h"

//===========================================================================
//	Single Float
//===========================================================================
void GW_File::save(const float& _inData, FILE* _file)
{
	double _temp = double(_inData);
	fwrite(&_temp,sizeof(double),1, _file);
}
void GW_File::load(float& _outData, FILE* _file)
{
	double _temp = 0;
	fread(&_temp,sizeof(double),1,_file);
	
	_outData = float(_temp);
}

//===========================================================================
//	Vectors
//===========================================================================
void GW_File::save(const gwVector& _inData, FILE* _file)
{
	double _temp[] = {double(_inData.x), double(_inData.y)};
	fwrite(_temp,sizeof(double),2,_file);
}

void GW_File::load(gwVector& _outData, FILE* _file)
{
	double _temp[2] = {0};
	fread(_temp,sizeof(double),2,_file);

	_outData.x = float(_temp[0]);
	_outData.y = float(_temp[1]);
}

//===========================================================================
//	Segments
//===========================================================================
void GW_File::save(const gwSegment& _inData, FILE* _file)
{
	double _temp[] =
	{
		double(_inData.sx),
		double(_inData.sy),
		double(_inData.ex),
		double(_inData.ey)
	};

	fwrite(_temp,sizeof(double),4,_file);
}

void GW_File::load(gwSegment& _outData, FILE* _file)
{
	double _temp[4] = {0};
	fread(_temp,sizeof(double),4,_file);

	_outData.sx = float(_temp[0]);
	_outData.sy = float(_temp[1]);
	_outData.ex = float(_temp[2]);
	_outData.ey = float(_temp[3]);
}

//===========================================================================
//	Map Object
//===========================================================================
void GW_File::save(gwMapObject* _inData, FILE* _file)
{
	int _type = _inData->Type() + GW_TYPE_OFFSET;
	fwrite(&_type,sizeof(int),1, _file);
	save(_inData->m_pos,_file);
	save(_inData->m_size,_file);

	switch(_inData->Type())
	{
		//--------------------------------
		//	Base Object
		//--------------------------------
	case GW_OBJECT:		break;
		//--------------------------------
		//	Spawner
		//--------------------------------
	case GW_OBJ_SPAWNER:
		{
			gwSpawner* _spawn = reinterpret_cast<gwSpawner*>(_inData);
			unsigned int _temp[] = {_spawn->m_max,_spawn->m_delay};
			fwrite(_temp,sizeof(unsigned int),2,_file);
		} break;
		//--------------------------------
		//	Home
		//--------------------------------
	case GW_OBJ_HOME:
		{
			gwHome* _home = reinterpret_cast<gwHome*>(_inData);
			fwrite(&(_home->m_max),sizeof(unsigned int),1,_file);
		} break;
	};
}

gwMapObject* GW_File::loadObject(FILE* _file)
{
	int _type = 0;
	fread(&_type,sizeof(int),1,_file);
	_type -= GW_TYPE_OFFSET;

	switch(_type)
	{
		//--------------------------------
		//	Base Object
		//--------------------------------
	case GW_OBJECT:
		{
			gwMapObject* _obj = new gwMapObject();
			load(_obj->m_pos,_file);
			load(_obj->m_size, _file);
			return _obj;
		}
		break;
		//--------------------------------
		//	Spawner
		//--------------------------------
	case GW_OBJ_SPAWNER:
		{
			gwSpawner* _spawn = new gwSpawner();
			load(_spawn->m_pos,_file);
			load(_spawn->m_size, _file);
			
			unsigned int _temp[2] = {0};
			fread(_temp,sizeof(unsigned int),2,_file);
			_spawn->m_max = _temp[0];
			_spawn->m_delay = _temp[1];

			return _spawn;
		}
		//--------------------------------
		//	HOME
		//--------------------------------
	case GW_OBJ_HOME:
		{
			gwHome* _home = new gwHome();
			load(_home->m_pos,_file);
			load(_home->m_size, _file);

			fread(&(_home->m_max),sizeof(unsigned int),1,_file);

			return _home;
		}
	};
	return 0;
}