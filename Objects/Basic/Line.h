#pragma once
class Line
{
public:	// 생성자, 소멸자
	Line(wstring shaderFile);
	~Line();

public: // 공유 함수
	void Update(Matrix V, Matrix P);
	void Render();
	void UpdateBlock();
	// Update, Render

	void SetPosition(float x, float y) { position = Vector2(x, y); }
	void SetScale(float sx, float sy) { scale = Vector2(sx, sy); }
	void SetRotation(float x, float y, float z) { rotation = Vector3(x, y, z); }
	// Setter 함수

private: // 비공유 함수
	void CreateVertexBuffer();

private: // 멤버 변수
	struct Vertex // 정점 구조체
	{
		Vector3 position;
		Color	color;
	};

	Vector2 position = Vector2(0.0f, 0.0f);
	Vector2 scale = Vector2(1.0f, 1.0f);
	Vector3 rotation = Vector3(0.0f, 0.0f, 0.0f);
	// 기본값으로 설정해준다.

	ID3D11Buffer* vertexBuffer = nullptr;
	class ColorShader* shader = nullptr;
	// 정점 버퍼와 셰이더
};

