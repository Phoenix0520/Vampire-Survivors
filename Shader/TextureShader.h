#pragma once
class TextureShader
{
public: // ������ �Ҹ���
	TextureShader(wstring shaderFile);
	~TextureShader();

public: // ���� �Լ�
	void Update(Matrix W, Matrix V, Matrix P);
	void UpdateColorBuffer(Color color, int path, float time, float time2, float time3);
	void Draw(UINT vertexCount, UINT startVertexLocation);
	void DrawIndexed(UINT indexCount, UINT startIndexLocation, int baseVertexLocation);
	// ���̴� �Լ�

	void SetShaderResourceView(ID3D11ShaderResourceView* srv) { this->srv = srv; }
	// Setter �Լ�

	auto GetLayout() { return inputLayout; }
	// Getter �Լ�

private: // ����� �Լ�
	void CreateShader(wstring shaderFile);
	void CreateInputLayout();
	void CreateConstantBuffer();		// Matrix ���� Constant Buffer ����
	void CreateConstantColorBuffer();	// Color cbuffer
	void CreateSampleState();
	void CreateBlendState();
	void CreateRasterState();
	// Create �Լ�

private: // ��� ����
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
	// Alpha Blending ���鿡 �׸��� ���� ������ ���ִ°��̴�.
	// OM �ܰ� ( A Object �� B Object �� �������� ��� ǥ�������� ���� �����̴�.
	ID3D11ShaderResourceView*	srv = nullptr;
	// Source Texture �� ����Ǿ� �̹��� ������ ��´�.

	// Texture

};
