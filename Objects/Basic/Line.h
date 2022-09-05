#pragma once
class Line
{
public:	// ������, �Ҹ���
	Line(wstring shaderFile);
	~Line();

public: // ���� �Լ�
	void Update(Matrix V, Matrix P);
	void Render();
	void UpdateBlock();
	// Update, Render

	void SetPosition(float x, float y) { position = Vector2(x, y); }
	void SetScale(float sx, float sy) { scale = Vector2(sx, sy); }
	void SetRotation(float x, float y, float z) { rotation = Vector3(x, y, z); }
	// Setter �Լ�

private: // ����� �Լ�
	void CreateVertexBuffer();

private: // ��� ����
	struct Vertex // ���� ����ü
	{
		Vector3 position;
		Color	color;
	};

	Vector2 position = Vector2(0.0f, 0.0f);
	Vector2 scale = Vector2(1.0f, 1.0f);
	Vector3 rotation = Vector3(0.0f, 0.0f, 0.0f);
	// �⺻������ �������ش�.

	ID3D11Buffer* vertexBuffer = nullptr;
	class ColorShader* shader = nullptr;
	// ���� ���ۿ� ���̴�
};

