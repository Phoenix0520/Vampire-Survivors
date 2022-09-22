#pragma once

class Player : public GameObject
{
public:
	Player();
	~Player();

public:
	enum State
	{
		IDLE = 0,
		MOVE,
		ATTACKED
	};

public:
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset()  override;

	
	void	MoveObject();
	void	ItemAttack();
	// 행동

	void	EatChicken() { if (hp > maxHp - 30.0f) hp = maxHp; else hp += 30.0f; };
	void	UseRosary();
	void	UseNduja();
	void	UseClock();
	void	UseChest();

	// 아이템 사용

	void	EquipBasic();
	void	ResetAbility();
	void	ApplyItem(int id);
	// 기본 적용

	float	SetGUI(int id);
	RECT	GetCollisionRect();
	bool	IsMonsterInRegion(class Monster* mob);
	vector<UINT> GetMobsInScreen() { return mobsInScreen; }
	void SetMobsInScreen();
	class ScreenEffect* GetScreenEffect() { return screenEffect; }
	

	void	SetGold			(int val)	{ gold = val; }
	void	SetPower		(float val) { power = val; }
	void	SetDef			(float val) { def = val; }
	void	SetMaxHp		(float val) { maxHp = val; }
	void	SetRecovery		(float val) { recovery = val; }
	void	SetCoolDown		(float val) { coolDown = val; }
	void	SetArea			(float val) { area = val; }
	void	SetSkillSpeed	(float val) { skillSpeed = val; }
	void	SetDuration		(float val) { duration = val; }
	void	SetAmount		(float val) { amount = val; }
	void	SetMoveSpeed	(float val) { moveSpeed = val; }
	void	SetMagnet		(float val) { magnet = val; }
	void	SetLuck			(float val) { luck = val; }
	void	SetGrowth		(float val) { growth = val; }

	int		GetGold()		{ return gold; }
	float	GetPower()		{ return power; }
	float	GetDef()		{ return def; }
	float	GetMaxHp()		{ return maxHp; }
	float 	GetRecovery()	{ return recovery; }
	float	GetCoolDown()	{ return coolDown; }
	float	GetArea()		{ return area; }
	float	GetSkillSpeed() { return skillSpeed; }
	float	GetDuration()	{ return duration; }
	float	GetAmount()		{ return amount; }
	float	GetMoveSpeed()	{ return moveSpeed; }
	float	GetMagnet()		{ return magnet; }
	float	GetLuck()		{ return luck; }
	float	GetGrowth()		{ return growth; }

	void	LevelUp() { level++; }
	void	SetLevel(int val) { level = val; }
	void	SetExp(float val) { exp = val; }
	void	AddExp(float val);

	int		GetLevel() { return level; }
	float	GetExp() { return exp; }

	int		GetCoin()			{ return coin; }
	void	SetCoin(int val)	{ coin = val; }

	void	SetHp(float val) { hp = val; }
	float	GetHp() { return hp; }

	int		GetKill() { return kill; }
	void	SetKill(int val) { kill = val; }

	int		GetLvUpRemain() { return lvUpRemain; }
	void	SetLvUpRemain(int val) { lvUpRemain = val; }

	int*	GetEquipedSkillList() { return equipedSkillList; }
	void	SetEquipedSkill(int id, int val) { equipedSkillList[id] = val; }

	int*	GetEquipedItemList() { return equipedItemList; }
	void	SetEquipedItem(int id, int val) { equipedItemList[id] = val; }

	Vector2 GetMovePos() { return oldmPos; }
	Vector2 GetOldMovePos() { return oldmPos; }

	float*	GetTotalDamageList() { return totalDamage; }
	void	AddTotalDamage(int id, float val) { totalDamage[id] += val; }

	int		GetFinalMin() { return finalMin; }
	void	SetFinalMin(int val) { finalMin = val; }

	int		GetFinalSec() { return finalSec; }
	void	SetFinalSec(int val) { finalSec = val; }

	int*	GetPrimaryEquipMin() { return primaryEquipMin; }
	void	SetPrimaryEquipMin(int id, int val) { primaryEquipMin[id] = val; }

	int*	GetPrimaryEquipSec() { return primaryEquipSec; }
	void	SetPrimaryEquipSec(int id, int val) { primaryEquipSec[id] = val; }

private:
	class Animation* animation = nullptr;
	class Texture* texture[10];
	class Texture* hpBar[2];
	class EquipList* skills = nullptr;
	class BloodEffect* bloodEffect[10];
	class ScreenEffect* screenEffect = nullptr;
	class FireEffect* fireEffect = nullptr;

	// Basic
	State	state = IDLE;
	Vector2 movePos = Vector2(0.0f, 0.0f);
	Vector2 oldmPos = Vector2(0.0f, 0.0f);
	bool	moving = false;
	bool    render = true;
	int		play = 0;
	int		blood = 0;
	float   val = 0.0f;
	float	ctime = 0.0f;
	float	time = 0.0f;

	// Stat
	int		gold = 0;
	float	power = 0;
	float	def = 1;
	float	maxHp = 120.0f;
	float	recovery = 0.0f;
	float	coolDown = 0.0f;
	float	area = 0;
	float	skillSpeed = 0;
	float	duration = 0;
	float	amount = 0;
	float	moveSpeed = 0;
	float	magnet = 30;
	float	luck = 0;
	float	growth = 0;

	// InGame

	int level = 1;
	int lvUpRemain = 0;
	int coin = 0;
	int kill = 0;
	int skillAmount = 0;
	int itemAmount = 0;
	int equipedSkillList[6] = { -1, -1, -1, -1, -1, -1 };
	int equipedItemList[6] = { -1, -1, -1, -1, -1, -1 };

	float hp = 120.0f;
	float exp = 0.0f;

	float totalDamage[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	int primaryEquipMin[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int primaryEquipSec[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	int finalMin = 0;
	int finalSec = 0;

	vector<UINT> mobsInScreen;
};