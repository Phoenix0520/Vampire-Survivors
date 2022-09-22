#pragma once

/////////////////////////////////////////////////
// 
// ScreenEffect Ŭ����
//
// Type 0 : NONE
// Type 1 : ȭ���� ��½�̴� ȿ���� �ش�. Rosary ���� ����
// Type 2 : ȭ�鿡 Ǫ������ ������ Ǫ������ �ð� ������ �׸��� ȸ���Ѵ� Clock ���� ����
// ���� ���� : 2022 / 07 / 03
// ���� ���� : x
//
/////////////////////////////////////////////////
class ScreenEffect : public GameObject
{
public:
	ScreenEffect();
	~ScreenEffect();

public:
	enum Type
	{
		NONE = 0,
		GLOW_EFFECT,
		ICE_EFFECT
	};

public:
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;

	void SetType(int val) { type = (Type)val; }
	void SetType(Type val) { type = val; }
	Type GetType() { return type; }

	float GetFrozenTime() { return ftime; }

	bool IsMonsterInScreen(class Monster* mob);

private:
	class Texture* background = nullptr; // ȭ�� ��ü�� ������ �ִ� �ؽ���
	class Texture* texture = nullptr; // �߰� ȿ���� ��� �ؽ���

	class Player* pl = nullptr;

	float ftime = 10.0f;
	float time = 0.0f; // ȿ���� �ֱ����� �ð�
	float alpha = 0.0f; // ȿ���� ���Ǵ� ���İ�

	Type type = NONE;
};