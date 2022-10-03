#pragma once

class Camera // �̱��� ������ ����
{
private:
	Camera();
	~Camera();
	// �ܺο��� ���� �� �Ҹ� �Ұ�
public:
	static Camera* GetInstance()
	{
		static Camera instance;
		return &instance;
	}
	// Singleton Instance ��ȯ

	void PreUpdate();	// ī�޶� �����ҽ� PreUpdate ���� ��ġ ����
	void Update();		// ī�޶��� ������Ʈ

	void VCToWC(Vector2& position);	// View -> Window(2D)
	void WCToVC(Vector2& position);	// Window -> View
	// ����Լ�

public:
	void SetPosition(float x, float y) { position = Vector2(x, y); }
	void SetPosition(Vector2 pos) { position = pos; }
	void SetObject(class GameObject* obj) { object = obj; } 
	// �ش� ������Ʈ�� ī�޶� ����
	// Setter
	
	Matrix	GetViewMatrix() { return view; }
	Matrix	GetProjMatrix() { return proj; }
	Vector2 GetPosition() { return position; }
	// Getter

private: // ��� ����
	class GameObject*	object	= nullptr;	// ī�޶� ������ �� ������Ʈ

	Vector2	position	= Vector2(0.0f, 0.0f);	// ī�޶��� ��ġ
	Vector2 offset		= Vector2(0.0f, 0.0f);	// ī�޶� ������ ��ġ ������
	Vector2 moveSpeed	= Vector2(200.0f, 200.0f); // Camera ���ۼӵ�
	Matrix	proj;	// Camera �� Projection Matrix
	Matrix	view;	// Camera �� View Matrix

};