#pragma once

class EquipList : public GameObject
{
public:
	EquipList();
	~EquipList();

public:
	struct Skill // ��ų�� ���� Desc
	{
		string name; // �̸�
		
		class SkillEffect* sEffect = nullptr;
		// ��ų ����Ʈ

		int id			= 0;	// ID
		int level		= 0;	// ����
		int amount		= 1;	// ����ü ����
		int spear		= 0;	// 0 : All | 1 ~ : Spear Amount
		float damage	= 0.0f;	// ������
		float speed		= 0.0f; // ����ü �ӵ�
		float area		= 0.0f; // ����ü ����
		float coolTime	= 0.0f; // ��Ÿ�� ����
		float duration  = 0.0f; // ���ӽð�
		bool equip		= false; // ��������
	};
	
	struct Item // �������� Desc
	{
		string name; // �̸�
		
		int id			= 0; // ID
		int level		= 0; // ����
		int maxLv		= 0; // ���� ����
		float value		= 0; // ������ ���밪
		float totalVal	= 0; // �� ���밪
		bool equip		= false; // ���� ����
	};

public:
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;
	// Display
	
	void ReadTextFile			(int row, Vector2& offset, Vector2& offsetSize);
	void SetRenderType			(int val) { type = val; }
	// General
	
	void AssignSkill			(string name, float damage, float coolTime, float duration = 0.0f, int spear = 0);
	void AssignItem				(string name, float value, int maxLv);

	void UpdateSkillEffect		(int id, SkillEffect* val) { skills[id]->sEffect = val; }
	void UpdateSkillAmount		(int id, int val) { skills[id]->amount = val; }
	void UpdateSkillSpear		(int id, int val) { skills[id]->spear = val; }
	void UpdateSkillLevel		(int id, int val) { skills[id]->level = val; }
	void UpdateSkillDamage		(int id, float val) { skills[id]->damage = val; }
	void UpdateSkillSpeed		(int id, float val) { skills[id]->speed = val; }
	void UpdateSkillArea		(int id, float val) { skills[id]->area = val; }
	void UpdateSkillCoolTime	(int id, float val) { skills[id]->coolTime = val; }
	void UpdateSkillDuration	(int id, float val) { skills[id]->duration = val; }
	void UpdateSkillEquip		(int id, bool val) { skills[id]->equip = val; }

	void AddSkillLevel			(int id, int val) { skills[id]->level += val; }
	void AddSkillAmount			(int id, int val) { skills[id]->amount += val; }
	void AddSkillSpear			(int id, int val) { skills[id]->spear += val; }
	void AddSkillDamage			(int id, float val) { skills[id]->damage += val; }
	void AddSkillSpeed			(int id, float val) { skills[id]->speed += val; }
	void AddSkillArea			(int id, float val) { skills[id]->area += val; }
	void AddSkillCoolDown		(int id, float val) { skills[id]->coolTime -= val; }
	void AddSkillDuration		(int id, float val) { skills[id]->duration += val; }
	// Setter

	SkillEffect* GetSkillEffect (int id) { return skills[id]->sEffect; }
	int	  GetSkillLevel			(int id) { return skills[id]->level; }
	int	  GetSkillAmount		(int id) { return skills[id]->amount; }
	int	  GetSkillSpear			(int id) { return skills[id]->spear; }
	float GetSkillDamage		(int id) { return skills[id]->damage; }
	float GetSkillSpeed			(int id) { return skills[id]->speed; }
	float GetSkillArea			(int id) { return skills[id]->area; }
	float GetSkillCoolTime		(int id) { return skills[id]->coolTime; }
	float GetSkillDuration		(int id) { return skills[id]->duration; }
	bool  GetSkillEquip			(int id) { return skills[id]->equip; }
	// Getter

	void UpdateItemValue		(int id, float val) { items[id]->value = val; }
	void UpdateItemTotalValue	(int id) { items[id]->totalVal = items[id]->level * items[id]->value; }
	void UpdateItemLevel		(int id, int val) { items[id]->level = val; }
	void UpdateItemMaxLevel		(int id, int val) { items[id]->maxLv = val; }
	void UpdateItemEquip		(int id, bool val) { items[id]->equip = val; }

	float GetItemValue			(int id) { return items[id]->value; }
	float GetItemTotalValue		(int id) { return items[id]->totalVal; }
	int   GetItemLevel			(int id) { return items[id]->level; }
	int   GetItemMaxLevel		(int id) { return items[id]->maxLv; }
	bool  GetItemEquip			(int id) { return items[id]->equip; }

	int   GetEquipLevel(int id) { if (id >= 8) return items[id - 8]->level; else return skills[id]->level; }

private:
	vector<Skill*> skills;
	vector<Item*> items;

	class Player* player = nullptr;
	class Texture* texture[2];
	class Texture* tile = nullptr;
	class Texture* item = nullptr;
	class Texture* levelVal = nullptr;

	int type = 0;

};