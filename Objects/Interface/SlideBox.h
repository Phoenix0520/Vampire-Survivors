#pragma once

class SlideBox : public UIObject
{
public:
	SlideBox();
	~SlideBox();

public:
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;

	float GetPercent() { return percent; }
	void SetPercent(float val) { percent = val; }

private:
	class Texture* texture = nullptr;
	class Rect* rect = nullptr;
	class Cursor* cursor = nullptr;

	float percent = 10.0f;
};