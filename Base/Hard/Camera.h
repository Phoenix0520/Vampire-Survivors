#pragma once

class Camera // 싱글톤 디자인 패턴
{
private:
	Camera();
	~Camera();
	// 외부에서 생성 및 소멸 불가
public:
	static Camera* GetInstance()
	{
		static Camera instance;
		return &instance;
	}
	// Singleton Instance 반환

	void PreUpdate();	// 카메라 조작할시 PreUpdate 에서 위치 조정
	void Update();		// 카메라의 업데이트

	void VCToWC(Vector2& position);	// View -> Window(2D)
	void WCToVC(Vector2& position);	// Window -> View
	// 기능함수

public:
	void SetPosition(float x, float y) { position = Vector2(x, y); }
	void SetPosition(Vector2 pos) { position = pos; }
	void SetObject(class GameObject* obj) { object = obj; } 
	// 해당 오브젝트에 카메라 고정
	// Setter
	
	Matrix	GetViewMatrix() { return view; }
	Matrix	GetProjMatrix() { return proj; }
	Vector2 GetPosition() { return position; }
	// Getter

private: // 멤버 변수
	class GameObject*	object	= nullptr;	// 카메라 고정이 된 오브젝트

	Vector2	position	= Vector2(0.0f, 0.0f);	// 카메라의 위치
	Vector2 offset		= Vector2(0.0f, 0.0f);	// 카메라 고정시 위치 오프셋
	Vector2 moveSpeed	= Vector2(200.0f, 200.0f); // Camera 조작속도
	Matrix	proj;	// Camera 의 Projection Matrix
	Matrix	view;	// Camera 의 View Matrix

};