#ifndef CHECKERS_GAME_23454
#define CHECKERS_GAME_23454

#define CK_SETUP	1
#define CK_PLAY		2
#define CK_COMPLETE	3

#define CK_EMPTY	0
#define CK_RED		1
#define CK_BLACK	2
#define CK_RKING	3
#define CK_BKING	4

#define CK_WIDTH	8

class Checkers
{
public:
	Checkers();
	~Checkers();

	void Setup();
	void Move(int _srcX, int _srcY, int _destX, int _destY);

	bool CheckPiece(int x, int y) const;
	bool CheckMove(int _srcX, int _srcY, int _destX, int _destY) const;

	const bool getTurn() const;
	const int getState() const;
	const int* getBoard() const;

private:
	void checkGame();

	int	m_board[64];
	bool m_turn;
	int m_state;
};

#endif