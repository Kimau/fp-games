#ifndef FP_GAME_TILE_23564
#define FP_GAME_TILE_23564

class gameTile
{
public:
	gameTile();
	~gameTile();

	bool Empty();
	void Generate();
	void RotateCW();
	void RotateCCW();

	int		m_north;
	int		m_south;
	int		m_east;
	int		m_west;
};

gameTile::gameTile()
{
	Generate();
}

gameTile::~gameTile()
{
}

bool gameTile::Empty()
{
	if(m_north > 0)		return false;
	if(m_east > 0)		return false;
	if(m_south > 0)		return false;
	if(m_west > 0)		return false;
	return true;
}

void gameTile::Generate()
{
	m_north = rand() % 3;
	m_east = rand() % 3;
	m_south = rand() % 3;
	m_west = rand() % 3;
}

void gameTile::RotateCW()
{
	int _temp = m_north;
	m_north = m_west;
	m_west = m_south;
	m_south = m_east;
	m_east = _temp;
}

void gameTile::RotateCCW()
{
	int _temp = m_north;
	m_north = m_east;
	m_east = m_south;
	m_south = m_west;
	m_west = _temp;
}

#endif