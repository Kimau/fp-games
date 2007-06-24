#ifndef HEADER_GAME_OBJECT_23423435325
#define HEADER_GAME_OBJECT_23423435325

#define GOBJ_BASIC 10000

//------------------------
//	Game Object Struct
//------------------------
class GObject
{
public:
	//------------------------
	//	Constructors
	//------------------------
	GObject();
	GObject(const GObject& _other);
	~GObject();

	//------------------------
	//	Operators
	//------------------------
	virtual GObject& operator= (const GObject& _other);
	virtual bool operator< (const GObject& _other) const;
	virtual bool operator> (const GObject& _other) const;
	virtual bool operator== (const GObject& _other) const;

	//------------------------
	// Functions
	//------------------------
	virtual void Update(unsigned long _delta);
	virtual unsigned int getType() const;

	void setMoment(float* _moment);
	void setMoment(	float _posX, float _posY, float _posR,
					float _velX, float _velY, float _velR,
					float _aclX, float _aclY, float _aclR);

	//------------------------
	//	Accesors
	//------------------------
	const unsigned int Id();
	//------------------------
	//	Varibles
	//------------------------
	float m_pos[3];		// Position		(x,y, rotation)
	float m_vel[3];		// Veleocity
	float m_acl[3];		// Acceleration
	float m_size[2];
private:
	unsigned int m_type;
	unsigned int m_id;

	static unsigned int getNewID();
};

#endif


