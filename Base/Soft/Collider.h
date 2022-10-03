#pragma once

class Collider	// �浹�� üũ�ϱ� ���� Ŭ����
{
public:
	Collider();	// ������
	~Collider(); // �Ҹ���

public:
	struct Vertex // ���� �׸��� ���� ����
	{
		Vector3 Position;
		Color   Color;
	};

public:
	void Update(Matrix View, Matrix Projection); // ������ ũ��� ������ ����ش�.
	void Render(); // ���� ����Ѵ�.

	static bool InterSect(class Collider *A, class Collider *B); // AABB �浹üũ

public:
	void		    SetPosition(float x, float y) { position = Vector2(x, y); }
	void		    SetPosition(Vector2 pos) { position = pos; }
	void		    SetScale(float x, float y) { scale = Vector2(x, y); };
	void		    SetScale(Vector2 size) { scale = size; };
	void		    SetRotation(float x, float y, float z) { rotation = Vector3(x, y, z); };
	void		    SetRotation(Vector3 rot) { rotation = rot; };
	void            SetWorld(Matrix value) { world = value; }
	// Setter �Լ�

	Vector2			GetPosition() { return position; }
	Vector2			GetScale() { return scale; }
	Vector3			GetRotation() { return rotation; }
	Matrix			GetWorld() { return world; }
	// Getter �Լ�

private:
	void                CreateBuffer(); // Vertex Buffer ����

private:
	class  ColorShader* shader = nullptr;		// ���� �׸��� ���� shader
	ID3D11Buffer*	vertexBuffer = nullptr;		// ���� �׸��� ���� ����
	Vector2			position = Vector2(0.0f, 0.0f);			// GameObject ��ǥ
	Vector2			scale = Vector2(1.0f, 1.0f);			// GameObject ũ��
	Vector3			rotation = Vector3(0.0f, 0.0f, 0.0f);	// GameObject ���� 
	Matrix			world;											// GameObject WorldMatrix  

};