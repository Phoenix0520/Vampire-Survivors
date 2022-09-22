#pragma once

/////////////////////////////////////////////////
// 
// ScreenEffect 클래스
//
// Type 0 : NONE
// Type 1 : 화면이 번쩍이는 효과를 준다. Rosary 에서 사용됨
// Type 2 : 화면에 푸른빛이 감돌고 푸른빛이 시계 방향을 그리며 회전한다 Clock 에서 사용됨
// 제작 일자 : 2022 / 07 / 03
// 변경 내용 : x
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
	class Texture* background = nullptr; // 화면 전체에 영향을 주는 텍스쳐
	class Texture* texture = nullptr; // 추가 효과를 담는 텍스쳐

	class Player* pl = nullptr;

	float ftime = 10.0f;
	float time = 0.0f; // 효과를 주기위한 시간
	float alpha = 0.0f; // 효과에 사용되는 알파값

	Type type = NONE;
};