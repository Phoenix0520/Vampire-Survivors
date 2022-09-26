#pragma once

class CheckBox : public UIObject
{
public:
	CheckBox();
	~CheckBox();

public:
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;
	bool IsPointInRegion(Vector2 position);
	bool IsCheck() { return check; }

private:
	class Texture* texture = nullptr;
	class Cursor* cursor = nullptr;	

	int  check = false;
};