#pragma once

class InGameUI : public GameObject
{
public:
	InGameUI();
	~InGameUI();

public:
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;

	int GetMin()		{ return min; }
	int GetSec()		{ return sec; }
	float GetRatio()	{ return ratio; }

	void SetMin(int val)		{ min = val; }
	void SetSec(int val)		{ sec = val; }
	void SetRatio(float val)	{ ratio = val; }
	

private:
	void ReadTextFile(int row, Vector2& offset, Vector2& offsetSize);

private:
	class Texture* texture[2];
	class Texture* levelBar = nullptr;
	class Texture* levelVal = nullptr;
	class Texture* equipedBox = nullptr;
	class Texture* item = nullptr;

	int min = 0;
	int sec = 0;
	float ratio = 1.0f;

	float time = 0.0f;
	float ctime = 0.0f;
	bool plus = true;

};