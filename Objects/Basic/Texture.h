#pragma once

class Texture
{
public:	// ������, �Ҹ���, ����ü
	Texture(wstring imageFile, wstring shaderFile);
	Texture(wstring imageFile, wstring shaderFile, float startX, float startY, float endX, float endY);
	~Texture();

	struct Vertex // ���� ����ü
	{
		Vector3 position;
		Vector2 uv;
	};

	struct SpriteResource
	{
		ID3D11ShaderResourceView* srv;
		Vector2					  spriteSize;
	};


public:	// ���� �Լ�
	void Update(Matrix V, Matrix P);
	void Render();
	void UpdateBlock();	// uv �� ����
	void UpdateColorBuffer(Color color, int path, float time, float time2, float time3);
	// Update, Render

	void CreateShaderResourceView(wstring imageFile); // Create Buffers, �ڿ���
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
	// Setter �Լ�

	Vector2 GetPosition() { return position; }
	Vector2 GetScale() { return scale; }
	Vector3 GetRotation() { return rotation; }
	Vector2 GetTextureRealSize();
	// Getter �Լ�

	Vector2 FindSpriteSize();

private: // ����� �Լ�
	void CreateVertexBuffer(float startX, float startY, float endX, float endY);
	// ������ �κ� ������ �׸��� �ְ� �Ѵ�.
	void CreateIndexBuffer();
	
	ID3D11ShaderResourceView* FindShaderResourceView();
	// srv �� ��ȯ�ϴ� �Լ�
	

private: // ��� ����
	// Texture ������ uv �� ����Ѵ�.

	map<wstring, SpriteResource*> spriteResources;
	// ImageFile �� ���� ����������

	Vector2 position = Vector2(0.0f, 0.0f);
	Vector2 scale = Vector2(1.0f, 1.0f);
	Vector3 rotation = Vector3(0.0f, 0.0f, 0.0f);
	// ������ ���� ��

	Vector2 spriteOffset = Vector2(0.0f, 0.0f);
	Vector2 spriteSize = Vector2(0.0f, 0.0f);
	Vector2 imageSize = Vector2(0.0f, 0.0f);
	// Sprite �� ���� ��

	wstring imageFile;
	// map ���� srv �� ã������ key

	ID3D11ShaderResourceView* srv = nullptr;	// ������ ���ϰ� �����μ� ���
	ID3D11Buffer*			  vertexBuffer = nullptr;
	class TextureShader*	  shader = nullptr;
	// srv, ���� ����, ���̴�

};

