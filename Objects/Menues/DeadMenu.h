#pragma once

class DeadMenu : public GameObject
{
public:
	DeadMenu();
	~DeadMenu();

public:
	void Update(Matrix V, Matrix P) override;
	void Render();
	void Reset();

private:
	class Texture* background = nullptr;
	class Texture* gameOver = nullptr;
	class Button* button = nullptr;

	
};