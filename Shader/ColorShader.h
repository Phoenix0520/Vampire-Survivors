#pragma once

class ColorShader	// ������ ����ϴ� Ŭ����
{
public: // ������ �Ҹ���
	ColorShader(wstring shaderFile);
	~ColorShader();

public:	// ���� �Լ�
	void Update(Matrix W, Matrix V, Matrix P);
	void Draw(UINT VertexCount, UINT StartVertexLocation);
	void DrawIndexed(UINT IndexCount, UINT StartVertexLocation, int BaseVertexLocation);
	// Shader �����Լ�

	auto GetLayout() { return inputLayout; }
	// Getter �Լ�

private: // ����� �Լ�
	void CreateShader(wstring shaderFile);
	void CreateInputLayout();
	void CreateMatrixBuffer();	// Matrix cbuffer
	

private: // ��� ����
	struct TRANSFORM_DATA
	{
		Matrix world;
		Matrix view;
		Matrix proj;
	};

	ID3D11VertexShader* vertexShader = nullptr;
	ID3D10Blob*			vsBlob = nullptr;
	ID3D11PixelShader*	pixelShader = nullptr;
	ID3D10Blob*			psBlob = nullptr;
	ID3D11InputLayout*  inputLayout = nullptr;
	// Com Interface

	ID3D11Buffer*		matrixBuffer = nullptr;
	// cbuffer

};