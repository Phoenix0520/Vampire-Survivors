#include "Texture.h"

class Texture;

#pragma once
class Camera
{
private: // ½Ì±ÛÅæ »ý¼º ¼Ò¸êÀÚ
	Camera();
	~Camera();

public: // Àü¿ª ÇÔ¼ö
	static Camera* GetInstance()
	{
		static Camera instance;

		return &instance;
	}
	// Singleton Instance

	void PreUpdate(); // Keyboard
	void Update();
	void Update(Matrix V, Matrix P);
	void Render();
	// Update, Render

	void VCToWC(Vector2& position);	// View -> Window(2D)
	void WCToVC(Vector2& position);	// Window -> View
	void ChangeXY(float x, float y);
	void ChangeXY(Vector2 xy);


	Matrix	GetViewMatrix() { return view; }
	Matrix	GetProjMatrix() { return proj; }
	Vector2 GetPosition() { return position; }
	// Getter

	void SetPosition(float x, float y) { position = Vector2(x, y); }
	void SetPosition(Vector2 pos) { position = pos; }
	void SetObject(class GameObject* obj) { object = obj; }

	void SetCenterXLock(bool val) { centerXLock = val; }
	void SetCenterYLock(bool val) { centerYLock = val; }

	// Setter

private: // ¸â¹ö º¯¼ö
	Vector2	position	= Vector2(0.0f, 0.0f);
	Vector2 moveSpeed	= Vector2(200.0f, 200.0f);
	Vector2 offset		= Vector2(0.0f, 0.0f);
	Matrix	proj;
	Matrix	view;
	
	class Texture*		texture = nullptr;
	class GameObject*	object	= nullptr;	

	float changeX = 1.0f;
	float changeY = 1.0f;

	bool centerXLock = false;
	bool centerYLock = false;
	
};