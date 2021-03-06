#include "Checkers.h"
#include <iostream>

using namespace std;

void Render(Checkers& _game);
void InputMove(Checkers& _game);

//==================================================================
//	Main function
//==================================================================
int main()
{
	//------------------------
	//	Init
	//------------------------
	Checkers _game;
	_game.Setup();

	//------------------------
	//	Game Loop
	//------------------------
	while(true)
	{
		switch(_game.getState())
		{
		case CK_SETUP:
			{
				break;
			}			
		case CK_PLAY:
			{
				Render(_game);
				cout << "             \n";
				InputMove(_game);
				break;
			}
		case CK_COMPLETE:
			{
				Render(_game);
				cout << " \n \t \t GAME OVER! \n";

				if(_game.getTurn())
					cout << "\t \t BLACK WINS! \n";
				else
					cout << "\t \t RED WINS! \n";

				cin.get();
				_game.Setup();
				break;
			}
		};
	}

	return 1;
}

//==================================================================
//	Render Game
//==================================================================
void Render(Checkers& _game)
{
	//----------------------------
	//	Clear Screen
	//----------------------------
	system("CLS");
	const int* _board = _game.getBoard();

	if(_game.getTurn() == false)
		cout << "\t\t\t REDS TURN \n";
	else
		cout << "\t\t\t BLACKS TURN \n";

	//----------------------------
	//	Draw Game Board
	//----------------------------
	cout << "\t   \xC9\xCD\xCD\xCD\xCB\xCD\xCD\xCD\xCB\xCD\xCD\xCD\xCB\xCD\xCD\xCD\xCB\xCD\xCD\xCD\xCB\xCD\xCD\xCD\xCB\xCD\xCD\xCD\xCB\xCD\xCD\xCD\xBB \n";
	for(int y = 7; y > -1; --y)
	{
		cout << "\t "<< (y+1) << " ";
		for(int x = 0; x < 8; ++x)
		{
			cout << "\xBA";
			switch(_board[x + y*CK_WIDTH])
			{
			case CK_EMPTY:	cout << "   ";		break;
			case CK_RED:	cout << " O ";		break;
			case CK_BLACK:	cout << " @ ";		break;
			case CK_RKING:	cout << "( )";		break;
			case CK_BKING:	cout << "(@)";		break;
			};
		}
		cout << "\xBA \n";
		if(y == 0)
			cout << "\t   \xC8\xCD\xCD\xCD\xCA\xCD\xCD\xCD\xCA\xCD\xCD\xCD\xCA\xCD\xCD\xCD\xCA\xCD\xCD\xCD\xCA\xCD\xCD\xCD\xCA\xCD\xCD\xCD\xCA\xCD\xCD\xCD\xBC \n";
		else
			cout << "\t   \xCC\xCD\xCD\xCD\xCE\xCD\xCD\xCD\xCE\xCD\xCD\xCD\xCE\xCD\xCD\xCD\xCE\xCD\xCD\xCD\xCE\xCD\xCD\xCD\xCE\xCD\xCD\xCD\xCE\xCD\xCD\xCD\xB9 \n";
	}
	cout << "\t     1   2   3   4   5   6   7   8   \n";
}

//==================================================================
//	Process Input
//==================================================================
void InputMove(Checkers& _game)
{
	char _buffer[50];
	//-------------------------
	//	Select Piece
	//-------------------------
	int x = -1;
	int y = -1;

	cout << endl << "Select Piece (x,y):";
	cin >> _buffer;
	if((_buffer[0] == '(') && (_buffer[2] == ',') && (_buffer[4] == ')'))
	{
		x = _buffer[1] - '1';
		y = _buffer[3] - '1';
	}

	//----------------------------
	//	Validate Piece
	//----------------------------
	if(_game.CheckPiece(x,y) == false)
	{
		Render(_game);
		cout << "<<< Invalid Piece! >>>\n";
		InputMove(_game);
		return;
	}

	//-------------------------
	//	Move Direction
	//-------------------------
	int _dx = -1;
	int _dy = -1;

	cout << "Move where (x,y) or (C)ancel:";
	cin >> _buffer;
	if((_buffer[0] == '(') && (_buffer[2] == ',') && (_buffer[4] == ')'))
	{
		_dx = _buffer[1] - '1';
		_dy = _buffer[3] - '1';
	}
	//----------------------------
	//	Player Canceled Move
	//----------------------------
	else if((_buffer[0] == 'C') || (_buffer[0] == 'c'))
	{
		Render(_game);
		cout << "<<< Cancel Move! >>>\n";
		InputMove(_game);
		return;
	}

	//----------------------------
	//	Validate Move
	//----------------------------
	if(_game.CheckMove(x,y,_dx,_dy) == false)
	{
		Render(_game);
		cout << "<<<< Invalid Move! >>>\n";
		InputMove(_game);
		return;
	}
	//----------------------------
	//	Move Piece
	//----------------------------
	else
	{
		_game.Move(x,y,_dx,_dy);
	}
}