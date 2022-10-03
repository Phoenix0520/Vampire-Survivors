#pragma once

class Collider	// 충돌을 체크하기 위한 클래스
{
public:
	Collider();	// 생성자
	~Collider(); // 소멸자

public:
	struct Vertex // 선을 그리기 위한 정점
	{
		Vector3 Position;
		Color   Color;
	};

public:
	void Update(Matrix View, Matrix Projection); // 설정된 크기로 정점을 찍어준다.
	void Render(); // 선을 출력한다.

	static bool InterSect(class Collider *A, class Collider *B); // AABB 충돌체크

public:
	void		    SetPosition(float x, float y) { position = Vector2(x, y); }
	void		    SetPosition(Vector2 pos) { position = pos; }
	void		    SetScale(float x, float y) { scale = Vector2(x, y); };
	void		    SetScale(Vector2 size) { scale = size; };
	void		    SetRotation(float x, float y, float z) { rotation = Vector3(x, y, z); };
	void		    SetRotation(Vector3 rot) { rotation = rot; };
	void            SetWorld(Matrix value) { world = value; }
	// Setter 함수

	Vector2			GetPosition() { return position; }
	Vector2			GetScale() { return scale; }
	Vector3			GetRotation() { return rotation; }
	Matrix			GetWorld() { return world; }
	// Getter 함수

private:
	void                CreateBuffer(); // Vertex Buffer 생성

private:
	class  ColorShader* shader = nullptr;		// 선을 그리기 위한 shader
	ID3D11Buffer*	vertexBuffer = nullptr;		// 선을 그리기 위한 버퍼
	Vector2			position = Vector2(0.0f, 0.0f);			// GameObject 좌표
	Vector2			scale = Vector2(1.0f, 1.0f);			// GameObject 크기
	Vector3			rotation = Vector3(0.0f, 0.0f, 0.0f);	// GameObject 방향 
	Matrix			world;											// GameObject WorldMatrix  

};