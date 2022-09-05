#pragma once
class TextureShader
{
public: // 생성자 소멸자
	TextureShader(wstring shaderFile);
	~TextureShader();

public: // 공유 함수
	void Update(Matrix W, Matrix V, Matrix P);
	void UpdateColorBuffer(Color color, int path, float time, float time2, float time3);
	void Draw(UINT vertexCount, UINT startVertexLocation);
	void DrawIndexed(UINT indexCount, UINT startIndexLocation, int baseVertexLocation);
	// 셰이더 함수

	void SetShaderResourceView(ID3D11ShaderResourceView* srv) { this->srv = srv; }
	// Setter 함수

	auto GetLayout() { return inputLayout; }
	// Getter 함수

private: // 비공유 함수
	void CreateShader(wstring shaderFile);
	void CreateInputLayout();
	void CreateConstantBuffer();		// Matrix 관련 Constant Buffer 생성
	void CreateConstantColorBuffer();	// Color cbuffer
	void CreateSampleState();
	void CreateBlendState();
	void CreateRasterState();
	// Create 함수

private: // 멤버 변수
	struct TRANSFORM_DATA
	{
		Matrix world;
		Matrix view;
		Matrix proj;
	};

	struct COLOR_DATA
	{
		Color	color;
		int		path;
		float	time;
		float	dummy1;
		float	dummy2;
	};

	ID3D11VertexShader*			vertexShader = nullptr;
	ID3D10Blob*					vsBlob = nullptr;
	ID3D11PixelShader*			pixelShader = nullptr;
	ID3D10Blob*					psBlob = nullptr;
	ID3D11InputLayout*			inputLayout = nullptr;
	
	// Com Interface
	
	ID3D11Buffer*				matrixBuffer = nullptr;
	ID3D11Buffer*				colorBuffer = nullptr;


	// Shader

	ID3D11SamplerState*			samplerState = nullptr;
	// hlsl SamplerState
	ID3D11RasterizerState*		rasterState = nullptr;
	// Raster State (Pixel)
	ID3D11BlendState*			blendState = nullptr;
	// Alpha Blending 도면에 그리기 전에 블렌딩을 해주는것이다.
	// OM 단계 ( A Object 와 B Object 가 섞였을때 어떻게 표기할지에 대한 내용이다.
	ID3D11ShaderResourceView*	srv = nullptr;
	// Source Texture 와 연결되어 이미지 파일을 담는다.

	// Texture

};
