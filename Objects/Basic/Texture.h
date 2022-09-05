#pragma once

class Texture
{
public:	// 생성자, 소멸자, 구조체
	Texture(wstring imageFile, wstring shaderFile);
	Texture(wstring imageFile, wstring shaderFile, float startX, float startY, float endX, float endY);
	~Texture();

	struct Vertex // 정점 구조체
	{
		Vector3 position;
		Vector2 uv;
	};

	struct SpriteResource
	{
		ID3D11ShaderResourceView* srv;
		Vector2					  spriteSize;
	};


public:	// 공유 함수
	void Update(Matrix V, Matrix P);
	void Render();
	void UpdateBlock();	// uv 값 변경
	void UpdateColorBuffer(Color color, int path, float time, float time2, float time3);
	// Update, Render

	void CreateShaderResourceView(wstring imageFile); // Create Buffers, 자원뷰
	// Create

	void SetPosition(float x, float y) { position = Vector2(x, y); }
	void SetPosition(Vector2 size) { position = size; }
	void SetScale(float sx, float sy) { scale = Vector2(sx, sy); }
	void SetScale(Vector2 size) { scale = size; }
	void SetRotation(float x, float y, float z) { rotation = Vector3(x, y, z); }
	void SetRotation(Vector3 rot) { rotation = rot; }
	void SetOffset(float x, float y) { spriteOffset = Vector2(x, y); }
	void SetOffset(Vector2 size) { spriteOffset = size; }
	void SetOffsetSize(float x, float y) { spriteSize = Vector2(x, y); }
	void SetOffsetSize(Vector2 size) { spriteSize = size; }
	void SetImageFile(wstring file) { imageFile = file; }
	void SetImageSize(Vector2 size) { imageSize = size; }
	// Setter 함수

	Vector2 GetPosition() { return position; }
	Vector2 GetScale() { return scale; }
	Vector3 GetRotation() { return rotation; }
	Vector2 GetTextureRealSize();
	// Getter 함수

	Vector2 FindSpriteSize();

private: // 비공유 함수
	void CreateVertexBuffer(float startX, float startY, float endX, float endY);
	// 선택한 부분 까지만 그릴수 있게 한다.
	void CreateIndexBuffer();
	
	ID3D11ShaderResourceView* FindShaderResourceView();
	// srv 를 반환하는 함수
	

private: // 멤버 변수
	// Texture 에서는 uv 를 사용한다.

	map<wstring, SpriteResource*> spriteResources;
	// ImageFile 에 대한 데이터저장

	Vector2 position = Vector2(0.0f, 0.0f);
	Vector2 scale = Vector2(1.0f, 1.0f);
	Vector3 rotation = Vector3(0.0f, 0.0f, 0.0f);
	// 정점의 대한 값

	Vector2 spriteOffset = Vector2(0.0f, 0.0f);
	Vector2 spriteSize = Vector2(0.0f, 0.0f);
	Vector2 imageSize = Vector2(0.0f, 0.0f);
	// Sprite 에 대한 값

	wstring imageFile;
	// map 에서 srv 를 찾기위한 key

	ID3D11ShaderResourceView* srv = nullptr;	// 생성은 안하고 변수로서 사용
	ID3D11Buffer*			  vertexBuffer = nullptr;
	class TextureShader*	  shader = nullptr;
	// srv, 정점 버퍼, 셰이더

};

