#pragma once

class PopUpMenu : public UIObject
{
public:
	PopUpMenu();
	~PopUpMenu();

public:
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;

	void SetID(int val) { id = val; }
	
	class SlideBox* GetSlideBox(int id) { return slideBox[id]; }
	class CheckBox* GetCheckBox(int id) { return checkBox[id]; }

private:
	class Texture* window[2];
	class Texture* rect = nullptr;

	class Button* button[3];
	class SlideBox* slideBox[2];
	class CheckBox* checkBox[5];

	int id = 0;

	float time = 0.0f;
};