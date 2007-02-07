#include "checkers.h"
#include <string.h>
#include <stdlib.h>

//===================================================================
//	Checkers Constructor
//===================================================================
Checkers::Checkers()
{
	memset(m_board,0,sizeof(int)*64);
	m_turn = false;
	m_state = CK_SETUP;
}
//===================================================================
//	Deconstructor
//===================================================================
Checkers::~Checkers()
{
}

//===================================================================
//	Setup Game
//===================================================================
void Checkers::Setup()
{
	//-----------------------------
	//	Setup Board
	//-----------------------------
	memset(m_board,0,sizeof(int)*64);
	m_board[0 + 0*CK_WIDTH] = CK_RED;
	m_board[1 + 1*CK_WIDTH] = CK_RED;
	m_board[2 + 0*CK_WIDTH] = CK_RED;
	m_board[3 + 1*CK_WIDTH] = CK_RED;
	m_board[4 + 0*CK_WIDTH] = CK_RED;
	m_board[5 + 1*CK_WIDTH] = CK_RED;
	m_board[6 + 0*CK_WIDTH] = CK_RED;
	m_board[7 + 1*CK_WIDTH] = CK_RED;

	m_board[0 + 6*CK_WIDTH] = CK_BLACK;
	m_board[1 + 7*CK_WIDTH] = CK_BLACK;
	m_board[2 + 6*CK_WIDTH] = CK_BLACK;
	m_board[3 + 7*CK_WIDTH] = CK_BLACK;
	m_board[4 + 6*CK_WIDTH] = CK_BLACK;
	m_board[5 + 7*CK_WIDTH] = CK_BLACK;
	m_board[6 + 6*CK_WIDTH] = CK_BLACK;
	m_board[7 + 7*CK_WIDTH] = CK_BLACK;

	m_turn = false;
	m_state = CK_PLAY;
}

//===================================================================
//	Move Piece
//===================================================================
void Checkers::Move(int _srcX, int _srcY, int _destX, int _destY)
{
	//-----------------------------
	//	No moves allowed
	//-----------------------------
	if(m_state != CK_PLAY)
	{
		return;
	}
	//-----------------------------
	//	Check Valid Move
	//-----------------------------
	if(CheckMove(_srcX,_srcY,_destX,_destY) == false)
	{
		return;
	}

	//---------------------------
	//	Move Piece
	//---------------------------
	if((abs(_destX - _srcX) == 1) && (abs(_destY - _srcY) == 1))
	{
		m_board[_destX + _destY*CK_WIDTH] = m_board[_srcX + _srcY*CK_WIDTH];
		m_board[_srcX + _srcY*CK_WIDTH] = CK_EMPTY;
	}
	else if((abs(_destX - _srcX) == 2) && (abs(_destY - _srcY) == 2))
	{
		int _midX = _srcX + ((_destX - _srcX)/2);
		int _midY = _srcY + ((_destY - _srcY)/2);
		m_board[_destX + _destY*CK_WIDTH] = m_board[_srcX + _srcY*CK_WIDTH];
		m_board[_midX + _midY*CK_WIDTH] = CK_EMPTY;
		m_board[_srcX + _srcY*CK_WIDTH] = CK_EMPTY;
	}
	//---------------------------
	//	Check for Crowning
	//---------------------------
	if((_destY == 7) && (m_board[_destX + _destY*CK_WIDTH] == CK_RED))
	{
		m_board[_destX + _destY*CK_WIDTH] = CK_RKING;
	}
	else if((_destY == 0) && (m_board[_destX + _destY*CK_WIDTH] == CK_BLACK))
	{
		m_board[_destX + _destY*CK_WIDTH] = CK_BKING;
	}
	//---------------------------
	//	Change Turn
	//---------------------------
	m_turn = !m_turn;

	//--------------------------------
	//	Check if this move ended game
	//--------------------------------
	checkGame();
}

//===================================================================
//	Check Piece is Owned by Current Player
//===================================================================
bool Checkers::CheckPiece(int x, int y) const
{
	//---------------------------
	//	Check Bounds
	//---------------------------
	if((x < 0) || (x > 7) || (y < 0) || (y > 7))
	{
		return false;
	}

	//---------------------------
	//	Check Piece is Owned
	//---------------------------
	if(m_turn == false)
	{
		if((m_board[x + y*CK_WIDTH] == CK_RED) || (m_board[x + y*CK_WIDTH] == CK_RKING))
			return true;
	}
	else if((m_board[x + y*CK_WIDTH] == CK_BLACK) || (m_board[x + y*CK_WIDTH] == CK_BKING))
	{
		return true;
	}
	return false;
}

//===================================================================
//	Check if Move is Valid
//===================================================================
bool Checkers::CheckMove(int _srcX, int _srcY, int _destX, int _destY) const
{
	//----------------------------------
	//	Check you Own Piece
	//----------------------------------
	if(CheckPiece(_srcX,_srcY) == false)
	{
		return false;
	}

	//---------------------------
	//	Check Bounds
	//---------------------------
	if((_destX < 0) || (_destX > 7) || (_destY < 0) || (_destY > 7))
	{
		return false;
	}

	//---------------------------
	//	Check Direction
	//---------------------------
	switch(m_board[_srcX + _srcY*CK_WIDTH])
	{
	case CK_EMPTY:	
		return false;
	case CK_RED:
		if((_destY - _srcY) < 1)
			return false;
		break;
	case CK_BLACK:
		if((_srcY - _destY) < 1)
			return false;
		break;
	};
	
	//---------------------------
	//	Check Distance
	//---------------------------
	if((abs(_destX - _srcX) == 1) && (abs(_destY - _srcY) == 1))
	{
		//------------------------
		//	Empty Space
		//------------------------
		if(m_board[_destX + _destY*CK_WIDTH] == CK_EMPTY)
		{
			return true;
		}
	}
	else if((abs(_destX - _srcX) == 2) && (abs(_destY - _srcY) == 2))
	{
		int _midX = _srcX + ((_destX - _srcX)/2);
		int _midY = _srcY + ((_destY - _srcY)/2);
		//------------------------
		//	Empty Space
		//	And opp Piece to Jump
		//------------------------
		if(((m_board[_midX  + _midY*CK_WIDTH] % 2) == (m_turn+0)) &&
			(m_board[_destX + _destY*CK_WIDTH] == CK_EMPTY))
		{
			return true;
		}
	}
	//-------------------------
	//	Failed Check
	//-------------------------
	return false;
}

//===================================================================
//	Get Turn
//===================================================================
const bool Checkers::getTurn() const
{
	return m_turn;
}
//===================================================================
//	Get State
//===================================================================
const int Checkers::getState() const
{
	return m_state;
}
//===================================================================
//	Get Board
//===================================================================
const int* Checkers::getBoard() const
{
	return m_board;
}

//===================================================================
//	Check if Game is over
//===================================================================
void Checkers::checkGame()
{
	bool _bflag = true;
	bool _rflag = true;

	//------------------------------
	//	Search Board
	//------------------------------
	for(int c = 0; c < 64; ++c)
	{
		switch(m_board[c])
		{
			case CK_RED:
			case CK_RKING:
				_rflag = false;
				break;
			case CK_BLACK:
			case CK_BKING:
				_bflag = false;
				break;
		};
	}

	//------------------------------
	//	Check for Winner
	//------------------------------
	switch(_rflag + _bflag*2)
	{
	case 0: // Game Still in Play
		break;

	case 1: // Black Wins 
		m_turn = true;
		m_state = CK_COMPLETE;
		break;

	case 2:	// Red Wins
		m_turn = false;
		m_state = CK_COMPLETE;
		break;

	case 3:	// No Pieces on Board
		m_turn = false;
		m_state = CK_SETUP;
		break;
	};
}