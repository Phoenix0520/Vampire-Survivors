#pragma once

class Cursor : public GameObject
{
public:
	Cursor();
	~Cursor();
	
public:
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;

	void SetGap(float val) { gap = val; }
	void SetType(int val) { type = val; }

private:
	class Animation* animation[2];
	class Texture* texture1 = nullptr;
	class Texture* texture2 = nullptr;

	float gap = 0.0f;
	int type = 0;
};