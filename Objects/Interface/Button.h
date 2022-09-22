#pragma once

class Button : public UIObject
{
public:
	Button();
	~Button();

public:
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;

	void SetHover(bool val);
	void SetColor(int val) { color = val; }
	void SetID(int val) { id = val; }
	void SetButtonString(wstring str) { script = str; }
	void SetButtonStringSize(float val) { scriptSize = val; }
	void UpdateColorBuffer(Color color, int path, float a, float b, float c);

	bool IsPointInRegion(Vector2 position);


private:
	class Texture* texture = nullptr;

	int color = 0;
	float scriptSize = 0.0f;

	wstring script;
	class Cursor* cursor = nullptr;
	
};