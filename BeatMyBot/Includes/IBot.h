#pragma once
#include "World.h"

struct IBot
{
	virtual void Update(GameState* pTheGameState)=0;
	virtual void Initialise(MapData* pTheMapData, int teamNumber, int characterNumber)=0;
	virtual Vector2D GetVelocity() const=0;
	virtual bool GetIfFiring() const=0;
	virtual bool GetIfAiming() const=0;
	virtual bool GetIfThrowing() const=0;
	virtual int GetTargetTeam() const=0;
	virtual int GetTargetNumber() const=0;
	virtual Vector2D GetDebugPoint()=0;
	virtual Vector2D* GetDebugList()=0;
	virtual int GetDebugListLength()=0;
	virtual char* GetDebugData()=0;
};



	_declspec(dllexport) bool GetBotInterface(IBot ** pIBot);
//	typedef HRESULT (*GETINTERFACE) (IBot ** pIBot);

	_declspec(dllexport) bool FreeBotInterface(IBot ** pIBot);
//	typedef HRESULT (*FREEINTERFACE) (IBot ** pIBot);
