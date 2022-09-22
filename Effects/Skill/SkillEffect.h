#pragma once

class SkillEffect : public GameObject
{
public:
	SkillEffect();
	~SkillEffect();

public:
	void Update	(Matrix V, Matrix P) override;
	void Render	() override;
	void Reset	() override;

	//void UpdateSkillEffect1(Matrix V, Matrix P);
	//void UpdateSkillEffect2(Matrix V, Matrix P);
	//void UpdateSkillEffect3(Matrix V, Matrix P);
	//void UpdateSkillEffect4(Matrix V, Matrix P);
	//void UpdateSkillEffect5(Matrix V, Matrix P);
	//void UpdateSkillEffect6(Matrix V, Matrix P);
	//void UpdateSkillEffect7(Matrix V, Matrix P);
	//void UpdateSkillEffect8(Matrix V, Matrix P);

	bool IsMonsterInRegion(class Monster* mob, float advX = 0, float advY = 0);
	bool IsBrazierInRegion(class Brazier* obj, float advX = 0, float advY = 0);

	void SetID(int val)		{ id = val; }
	int	 GetID()			{ return id; }

protected:
	class Texture* texture = nullptr;

	class Player*		pl = nullptr;
	class EquipList*	sl = nullptr;

	vector<class Brazier*> brzs;

	int id			= 0;
	int spearCount	= 0;

	int	spear		= 0;

	float time		= 0.0f;
	float coolTime  = 0.0f;
	float area		= 0.0f;
	float speed		= 0.0f;
	float duration  = 0.0f;
	float damage	= 0.0f;
	// Skill Stats

	bool soundPlayed = false;
	vector<bool> attackedID;
	vector<UINT> ids;
};