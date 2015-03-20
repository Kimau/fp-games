#ifndef POINTXYZ
#define POINTXYZ

class pointXYZ
{
	public:

		pointXYZ(int, int, int);
		int getX() const;
		int getY() const;
		int getZ() const;
		bool operator==(const pointXYZ& other) const;
	
		pointXYZ();
  private:
		int x;
		int y;
		int z;
};

#endif

