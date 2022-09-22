#pragma once

class GameObject	// �߻�Ŭ����
{
public:	// ������, �Ҹ���
	GameObject() {}
	~GameObject() {}

public:
	enum Direction
	{
		left = 0,
		right,
		up,
		down
	};

public:	// ���� �����Լ�
	virtual void Update(Matrix V, Matrix P) = 0;
	virtual void Render() = 0;
	virtual void Reset() = 0;
	
public:	// ���� �Լ�
	void SetPosition(float x, float y)			{ position = Vector2(x, y); }
	void SetPosition(Vector2 size)				{ position = size; }
	void SetPosition(RECT rect)					{ position.x = (float)rect.right - (float)rect.left, position.y = (float)rect.top - (float)rect.bottom; }
	void SetScale(float sx, float sy)			{ scale = Vector2(sx, sy); }
	void SetScale(Vector2 size)					{ scale = size; }
	void SetRotation(float x, float y, float z) { rotation = Vector3(x, y, z); }
	void SetRotation(Vector3 rot)				{ rotation = Vector3(rot); }
	void SetActive(bool value)					{ active = value; }
	// Setter

	Vector2 GetPosition()						{ return position; }
	Vector2 GetScale()							{ return scale; }
	Vector3 GetRotation()						{ return rotation; }
	bool	IsActive()							{ return active; }
	class Collider* GetCollider()				{ return collider; }
	// Getter

protected:
	class Collider* collider = nullptr;

	Vector2 scale			= Vector2(1.0f, 1.0f);
	Vector2 position		= Vector2(0.0f, 0.0f);
	Vector3 rotation		= Vector3(0.0f, 0.0f, 0.0f);
	string  name;											// ������Ʈ�� �̸�
	bool	active			= true;							// ���ü��� �����Ѵ�.
	bool	collisionCheck	= false;						// �浹�ߴ��� �����Ѱ�.
};