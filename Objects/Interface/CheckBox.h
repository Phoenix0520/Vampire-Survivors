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

private:
	int  check = 0;
	class Cursor* cursor = nullptr;	
};