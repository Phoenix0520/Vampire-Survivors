#pragma once
class Rect
{
public:	// 생성자, 소멸자
	Rect(wstring shaderFile);
	~Rect();

public:	// 공유 함수
	void Update(Matrix V, Matrix P);
	void Render();
	void UpdateBlock(Color color);	// Color 값 변경
	// Update, Render

	void SetPosition(float x, float y) { position = Vector2(x, y); }
	void SetPosition(Vector2 pos) { position = pos; }
	void SetScale(float sx, float sy) { scale = Vector2(sx, sy); }
	void SetScale(Vector2 size) { scale = size; }
	void SetRotation(float x, float y, float z) { rotation = Vector3(x, y, z); }
	// Setter 함수

private: // 비공유 함수
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	// Create Buffers

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
	ID3D11Buffer* indexBuffer = nullptr;
	class ColorShader* shader = nullptr;
	// 정점 버퍼, 인덱스 버퍼, 셰이더

};