#ifndef __TYPES_HH__
# define __TYPES_HH__


# ifndef PI
#  define PI 3.14159265358f
# endif


// Integer colour class
class cICol
{
public:
	char R,G,B,A;
	cICol():R(0),G(0),B(0),A(0){}
	cICol(char lR,char lG,char lB):R(lR),G(lG),B(lB),A(0){}
	cICol(char lR,char lG,char lB,char lA):R(lR),G(lG),B(lB),A(lA){}

	// A few useful colour functions
	inline char val(){ return (char)((((float)R)+((float)G)+((float)B))*(1.0f/3.0f));}
	inline bool operator!=(const cICol& lCol)
		{ return (lCol.R!=R)||(lCol.G!=G)||(lCol.B!=B)||(lCol.A!=A);}
	inline bool operator==(const cICol& lCol)
		{ return (lCol.R==R)&&(lCol.G==G)&&(lCol.B==B)&&(lCol.A==A);}
};


// Float colour class
class cFCol
{
public:
	float R,G,B,A;
	cFCol():R(0.0f),G(0.0f),B(0.0f),A(0.0f){}
	cFCol(float lR,float lG,float lB):R(lR),G(lG),B(lB),A(0.0f){}
	cFCol(float lR,float lG,float lB,float lA):R(lR),G(lG),B(lB),A(lA){}

	// A few useful colour functions
	inline float val(){ return (R+G+B)*(1.0f/3.0f);}
};


// 2D vector
class cVec2
{
public:
	float X,Y;

	cVec2():X(0.0f),Y(0.0f){}
	cVec2(float lX,float lY):X(lX),Y(lY){}
	
	float mag();
	void norm();
	
	// Negative
	cVec2 operator-(){ return cVec2(-X,-Y);}
};


// 2D vector magnitude
inline float cVec2::mag()
{
	return sqrt((X*X)+(Y*Y));
}

// 2D vector normalise
inline void cVec2::norm()
{
	float lMag=mag();
	if (lMag!=0){ X/=lMag; Y/=lMag;}
}

// 2D vector addition
inline cVec2 operator+(cVec2 lA,cVec2 lB){ return cVec2(lA.X+lB.X,lA.Y+lB.Y);}

// 2D vector subtraction
inline cVec2 operator-(cVec2 lA,cVec2 lB){ return cVec2(lA.X-lB.X,lA.Y-lB.Y);}

// 2D vector*scalar
inline cVec2 operator*(cVec2 lV,float lS){ return cVec2(lV.X*lS,lV.Y*lS);}

// 2D vector/scalar
inline cVec2 operator/(cVec2 lV,float lS){ return cVec2(lV.X/lS,lV.Y/lS);}

// 2D vector dot product
inline float operator^(cVec2 lA,cVec2 lB){ return (lA.X*lB.X)+(lA.Y*lB.Y);}


// 3D vector
class cVec3
{
public:
	float X,Y,Z;

	cVec3():X(0.0f),Y(0.0f),Z(0.0f){}
	cVec3(float lX,float lY,float lZ):X(lX),Y(lY),Z(lZ){}
};


// 3D vector addition
inline cVec3 operator+(cVec3 lA,cVec3 lB){ return cVec3(lA.X+lB.X,lA.Y+lB.Y,lA.Z+lB.Z);}

// 3D vector*scalar
inline cVec3 operator*(cVec3 lV,float lS){ return cVec3(lV.X*lS,lV.Y*lS,lV.Z*lS);}

// 3D vector dot product
inline float operator^(cVec3 lA,cVec3 lB){ return (lA.X*lB.X)+(lA.Y*lB.Y)+(lA.Z*lB.Z);}


// 4x4 matrix
class cMtx16
{
public:
	float i[16];

	inline void ident()
	{
		i[0]=1.0f;  i[1]=0.0f;  i[2]=0.0f;  i[3]=0.0f;
		i[4]=0.0f;  i[5]=1.0f;  i[6]=0.0f;  i[7]=0.0f;
		i[8]=0.0f;  i[9]=0.0f;  i[10]=1.0f; i[11]=0.0f;
		i[12]=0.0f; i[13]=0.0f; i[14]=0.0f; i[15]=1.0f;
	}
	cMtx16()
	{
		ident();
	}
	cMtx16(float i0,  float i1,  float i2,  float i3,
			float i4,  float i5,  float i6,  float i7,
			float i8,  float i9,  float i10, float i11,
			float i12, float i13, float i14, float i15)
	{
		i[0]=i0;   i[1]=i1;   i[2]=i2;   i[3]=i3;
		i[4]=i4;   i[5]=i5;   i[6]=i6;   i[7]=i7;
		i[8]=i8;   i[9]=i9;   i[10]=i10; i[11]=i11;
		i[12]=i12; i[13]=i13; i[14]=i14; i[15]=i15;
	}
	// Transpose
	inline cMtx16 tx()
	{
		return cMtx16(i[0],i[4],i[8],i[12],
					i[1],i[5],i[9],i[13],
					i[2],i[6],i[10],i[14],
					i[3],i[7],i[11],i[15]);
	}
};


#endif
