#pragma once

#define MAX_MOB		50

class Monster : public GameObject
{
public:
	Monster();
	~Monster();

public:
	enum State
	{
		STOP = 0,
		MOVE,
		FROZEN,
		DEAD
	};

	enum Type
	{
		NONE = -1,
		BAT = 0,
		ZOMB = 2,
		SKULL = 4,
		GHOST = 6,
		GHOUL = 8,
		BEAST = 10,
		MANTIS = 12,
		MUMMY = 14,
		NEPANDES = 16,
		VINUS = 18
	};

public:
	void	Update(Matrix V, Matrix P) override;
	void	Render() override;
	void	Reset() override;
	// Update Render

	void	Attacked	(int id);
	// Object
	
	void	SetHp		(float val)	{ hp = val; }
	void	SetDamage	(float val)	{ damage = val; }
	void	SetExp		(float val) { exp = val; }
	void	SetMoveSpeed	(float val) { moveSpeed = val; }
	void	SetMoving	(bool val)  { moving = val; }
	void	SetStop		(bool val)	{ stop = val; }
	void	SetState	(UINT val)	{ state = (State)val; }
	void	SetType		(UINT val)	{ type = (Type)val; }
	void	SetIntersect(bool val)	{ intersect = val;}
	// Setter

	float	GetHp			()		{ return hp; }
	float	GetDamage		()		{ return damage; }
	float	GetExp			()		{ return exp; }
	float	GetMoveSpeed	()		{ return moveSpeed; }
	bool	GetStop			()		{ return stop; }
	bool	GetIntersect	()		{ return intersect; }
	UINT	GetState		()		{ return (UINT)state; }
	UINT	GetType			()		{ return (UINT)type; }
	RECT	GetCollisionRect();
	class Animation* GetAnimation() { return animation; }
	// Getter

private:
	class Animation* animation = nullptr;
	class HitEffect* hitEffect = nullptr;

	State state = MOVE;
	Type  type  = BAT;
	
	float ftime = 0.0f;
	float ctime = 0.0f;
	float time = 0.0f;

	float hp			= 0;
	float damage		= 0.0f;
	float exp			= 0;
	float kbDef			= 0.0f;
	float moveSpeed		= 0.0f;

	bool stop		= false;
	bool dead		= false;
	bool plus		= true;
	bool moving		= true;
	bool intersect	= true;

};