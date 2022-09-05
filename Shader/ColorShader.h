#pragma once

class ColorShader	// 색상을 담당하는 클래스
{
public: // 생성자 소멸자
	ColorShader(wstring shaderFile);
	~ColorShader();

public:	// 공유 함수
	void Update(Matrix W, Matrix V, Matrix P);
	void Draw(UINT VertexCount, UINT StartVertexLocation);
	void DrawIndexed(UINT IndexCount, UINT StartVertexLocation, int BaseVertexLocation);
	// Shader 구성함수

	auto GetLayout() { return inputLayout; }
	// Getter 함수

private: // 비공유 함수
	void CreateShader(wstring shaderFile);
	void CreateInputLayout();
	void CreateMatrixBuffer();	// Matrix cbuffer
	

private: // 멤버 변수
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