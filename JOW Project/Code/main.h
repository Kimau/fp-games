#ifndef MAIN_WINDOWS_FUCKING_STUPID_CLASS_2343
#define MAIN_WINDOWS_FUCKING_STUPID_CLASS_2343

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gl\glu.h>
#include <gl\gl.h>
#include "Structs.h"
#include "Helpers.h"
#include "MenuGL.h"

// Varibles
JWL_Helpers* g_Help = 0;

struct JWL_CHOICE
{
	char  m_name[50];
	char  m_scence[50];

	UINT  m_map;

	bool  m_ShowUnits;
	bool  m_ShowObjs;
};

// Function Predeclare
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CreateWin(HINSTANCE& hInstance, HWND& hwnd, int iCmdShow);
void GameLoop(HINSTANCE& hInstance, HWND& hwnd, JWL_CHOICE& hChoice);
void MenuLoop(HINSTANCE& hInstance, HWND& hwnd, JWL_CHOICE& hChoice);

#endif