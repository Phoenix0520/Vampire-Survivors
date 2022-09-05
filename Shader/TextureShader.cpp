#include "framework.h"
#include "TextureShader.h"

TextureShader::TextureShader(wstring shaderFile)
{
	CreateShader(shaderFile);
	CreateInputLayout();	// blob ����
	CreateConstantBuffer();
	CreateConstantColorBuffer();
	CreateSampleState();
	CreateBlendState();
	CreateRasterState();
}

TextureShader::~TextureShader()
{
	SAFE_RELEASE(blendState);
	SAFE_RELEASE(rasterState);
	SAFE_RELEASE(samplerState);
	SAFE_RELEASE(colorBuffer);
	SAFE_RELEASE(matrixBuffer);
	SAFE_RELEASE(inputLayout);
	SAFE_RELEASE(psBlob);
	SAFE_RELEASE(pixelShader);
	SAFE_RELEASE(vsBlob);
	SAFE_RELEASE(vertexShader);
}

///////////////////////////////////////////////////////////////////////////////////
//
//	ColorShader ������ ����� ����ϴ�.
//
//	
//	Create Shader �� ���� Blob �������� ����
//  -> �����ڿ� �ִ� ������� ����
//
//	Update : Map UnMap ���̿��� �ؽ��� ���泻�� ����
//
//
///////////////////////////////////////////////////////////////////////////////////

void TextureShader::Update(Matrix W, Matrix V, Matrix P)
{
	TRANSFORM_DATA temp;

	D3DXMatrixTranspose(&temp.proj, &P);
	D3DXMatrixTranspose(&temp.view, &V);
	D3DXMatrixTranspose(&temp.world, &W);

	// �ٲ��� �����͸� �����Ų��.

	// Buffer �� �����Ϸ��� Map �� UnMap ���̸� �����ؾ��Ѵ�.
	// �׷��� D3D11_MAPPED_SUBRESOURCE �Լ��� ����ؼ� �����Ѵ�.

	// Buffer�� �����Ϸ���  Map - UnMap()

	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	DeviceContext->Map(this->matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);

	// �����͸� ����
	memcpy_s(mapped_subresource.pData, mapped_subresource.RowPitch, &temp, sizeof(temp));

	DeviceContext->Unmap(this->matrixBuffer, 0);

}

void TextureShader::UpdateColorBuffer(Color color, int path, float time, float time2, float time3)
{
	// Color Data
	
	COLOR_DATA temp;

	// �ٲ��� ���ڷ� �����͸� �����Ų��.
	temp.color	= color;
	temp.path	= path;
	temp.time	= time;
	temp.dummy1 = time2;
	temp.dummy2 = time3;

	// �����͸� ����
	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	DeviceContext->Map(this->colorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
	memcpy_s(mapped_subresource.pData, mapped_subresource.RowPitch, &temp, sizeof(temp));
	DeviceContext->Unmap(this->colorBuffer, 0);

	// Pipeline �� �߰�
}

void TextureShader::Draw(UINT vertexCount, UINT startVertexLocation)
{
	// IA �� Object Class ���� �ذ�

	// VS �ܰ� Vertex Shader

	DeviceContext->VSSetShader(vertexShader, nullptr, 0);
	DeviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	// RS �ܰ� Raster Stage

	DeviceContext->RSSetState(rasterState);

	// PS �ܰ� Pixel Shader

	DeviceContext->PSSetShader(pixelShader, nullptr, 0);
	DeviceContext->PSSetShaderResources(0, 1, &srv);
	DeviceContext->PSSetSamplers(0, 1, &samplerState);
	DeviceContext->PSSetConstantBuffers(1, 1, &colorBuffer);
	// srv setting �� sampling

	// OM �ܰ� Output Merger

	DeviceContext->OMSetBlendState(blendState, nullptr, 0xffffffff);
	// AlphaBlending

	// Draw �� ���� ���������� �׸��� ������. -> ���������� ����

	DeviceContext->Draw(vertexCount, startVertexLocation);
	// ������ ����, �������� �Է��ؼ� Draw �� ���ش�.
}

void TextureShader::DrawIndexed(UINT indexCount, UINT startIndexLocation, int baseVertexLocation)
{

}

void TextureShader::CreateShader(wstring shaderFile)
{
	string str;
	str.assign(shaderFile.begin(), shaderFile.end()); // wstring to string

	if (_access(str.c_str(), 00) != 0)
	{
		str = "Texture Shader : File not found " + str;
		MessageBoxA(NULL, str.c_str(), "Game", MB_OK);
		return;
	}

	// ���� ���̴� ����
	{
		HRESULT hr = D3DX11CompileFromFile
		(
			shaderFile.c_str(),		// ���ϸ�
			0,					// MACRO ������
			0,					// Include ������
			"VS",				// hlsl ���Ͽ� ���ǵ� �Լ���� ����
			"vs_5_0",			// ���� ���̴� ����
			0,					// flag1
			0,					// flag2
			0,			// thread pump
			&vsBlob,			// blob
			0,			// �����޼���
			0				// �����޼��� �޴��ּ�
		);

		assert(SUCCEEDED(hr));

		hr = Device->CreateVertexShader
		(
			vsBlob->GetBufferPointer(),		// Blob �� ���� �ּ�
			vsBlob->GetBufferSize(),		// Blob �� ũ��
			nullptr,						//
			&vertexShader					// VS �� �ּ�
		);

		assert(SUCCEEDED(hr));
	}

	// �ȼ� ���̴� ����
	{
		HRESULT hr = D3DX11CompileFromFile
		(
			shaderFile.c_str(),		// ���ϸ�
			0,					// MACRO ������
			0,					// Include ������
			"PS",				// hlsl ���Ͽ� ���ǵ� �Լ���� ����
			"ps_5_0",			// ���� ���̴� ����
			0,					// flag1
			0,					// flag2
			nullptr,			// thread pump
			&psBlob,			// blob
			nullptr,			// �����޼���
			nullptr				// �����޼��� �޴��ּ�
		);

		assert(SUCCEEDED(hr));

		hr = Device->CreatePixelShader
		(
			psBlob->GetBufferPointer(),		// Blob �� ���� �ּ�
			psBlob->GetBufferSize(),		// Blob �� ũ��
			nullptr,						//
			&pixelShader					// PS �� �ּ�
		);

		assert(SUCCEEDED(hr));
	}
}

void TextureShader::CreateConstantBuffer()
{
	// hlsl ���Ͽ� TransformBuffer �� ���Եȴ�.
	// Constant Buffer, Matrix Buffer �� �Ȱ��� Buffer �̴�.

	// Buffer �� ����
	{
		// desc ����
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DYNAMIC;			// Data �� �뵵 : CPU Write & GPU Read. GPU ���� ������ �����ϴ�.
		desc.ByteWidth = sizeof(TRANSFORM_DATA);		// Data ���� �Է�
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// Buffer �� Ÿ��
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// Buffer �� CPU �� ����

		HRESULT hr = Device->CreateBuffer
		(
			&desc,
			nullptr,
			&matrixBuffer
		);

		assert(SUCCEEDED(hr));
	}
}

void TextureShader::CreateConstantColorBuffer()
{
	// Buffer �� ����
	{
		// desc ����
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DYNAMIC;				// Data �� �뵵 : CPU Write & GPU Read. GPU ���� ������ �����ϴ�.
		desc.ByteWidth = sizeof(COLOR_DATA);			// Data ���� �Է�
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// Buffer �� Ÿ��
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// Buffer �� CPU �� ����

		HRESULT hr = Device->CreateBuffer
		(
			&desc,
			nullptr,
			&colorBuffer
		);

		assert(SUCCEEDED(hr));
	}
}

void TextureShader::CreateInputLayout()
{
	// ���̾ƿ� ����
	{
		D3D11_INPUT_ELEMENT_DESC layout[2];

		// Struct Vector3
		{
			layout[0].SemanticName = "POSITION";					// �ø�ƽ�� ����
			layout[0].SemanticIndex = 0;							// �ø�ƽ�� �ѹ�
			layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;	// ����
			layout[0].InputSlot = 0;
			layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// ���������Ͱ��ȴ�.
			layout[0].AlignedByteOffset = 0;
			layout[0].InstanceDataStepRate = 0;
		}

		// Struct Color
		{
			layout[1].SemanticName = "TEXCOORD";					// �ø�ƽ�� ����
			layout[1].SemanticIndex = 0;							// �ø�ƽ�� �ѹ�
			layout[1].Format = DXGI_FORMAT_R32G32_FLOAT;	// ����
			layout[1].InputSlot = 0;
			layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// ���������Ͱ��ȴ�.
			layout[1].AlignedByteOffset = 12;							// float2 * 3 ( x, y, x) + w (����)
			layout[1].InstanceDataStepRate = 0;
		}

		// ������ ��� �׷������� ���Ѵ�.
		HRESULT hr = Device->CreateInputLayout
		(
			layout,						// desc name
			2,							// desc ����
			vsBlob->GetBufferPointer(), // ���������� �ּҸ� �ش�.
			vsBlob->GetBufferSize(),	// ������ ũ��
			&inputLayout
		);

		assert(SUCCEEDED(hr));
	}
}

void TextureShader::CreateSampleState()
{
	// desc ����
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));

	// desc ����
	{
		// UV ��ǥ�� W ��ǥ
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		// ����ȭ�� ��ǥ�� ��������� ��� �۵��� �������� ���� �÷���
		// WRAP = �����ȼ��� ������ �κ��� ä��ٴ� �ǹ��̴�.
		// CLAMP = ������ �ȼ��� ä���.
		// MIRROR = ������ �ȼ��� �������Ѽ� ä���.

		desc.BorderColor[0] = 1;
		desc.BorderColor[1] = 0;
		desc.BorderColor[2] = 0;
		desc.BorderColor[3] = 1;
		// �ܰ����� ��Ÿ����.

		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		// ���� ���� �����͸� ���ϴ� ����� ���ϴ� �÷���
		// ���� ���ð� 4���� ���� ���Ͽ� �����ų�� �ƴ����� �����Ѵ�.

		desc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		// Scaling ��� �� Ȯ�� �� �ɶ� ��հ��� ���� �̸� 1/4 ũ��� �̹����� �����Ѵ�.
		// MIN = �̹��� ���
		// MAG = �̹��� Ȯ��
		// POINT = �ΰ��� �ȼ��� �����Ѵٸ� ���ȼ��� ����Ѵ�.
		// LINEAR = �ΰ��� �ȼ��� ��հ�

		desc.MaxAnisotropy = 16;
		// ���漺 ���͸� (�̹漺 ���͸�)

		desc.MaxLOD = FLT_MAX;
		desc.MinLOD = FLT_MIN;
		// �Ÿ��� ���� �̹��� ����

		desc.MipLODBias = 0.0f;
	}

	// Sampler State ����
	{
		HRESULT hr = Device->CreateSamplerState
		(
			&desc,
			&samplerState
		);

		assert(SUCCEEDED(hr));
	}
}

void TextureShader::CreateBlendState()
{
	// Alpha ���� å���Ѵ�.

	// desc ����
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));

	// desc ����
	{
		// Blending �� OM �ܰ迡�� �����ȴ�.
		desc.AlphaToCoverageEnable = false;						// ���� �׵θ� ����
		desc.IndependentBlendEnable = true;							// ���� ȥ��

		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;		// 0.9 ����ϸ�
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	// 0.1 �̵Ǵ°��̴�.
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		// Blending �� ǥ�� Flags

	}

	// BlendState ����
	{
		HRESULT hr = Device->CreateBlendState
		(
			&desc,
			&blendState
		);

		assert(SUCCEEDED(hr));
	}
}

void TextureShader::CreateRasterState()
{
	// desc ����
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

	// desc ����
	{
		desc.FillMode = D3D11_FILL_SOLID;
		// ���θ� ĥ�ϴ� ��
		desc.CullMode = D3D11_CULL_NONE;
		// �޺κ��� ���� ĥ�Ұ����� ��� ��� ������ ���̰� �ϰڴٴ� �ǹ̴�.
		desc.FrontCounterClockwise = false;
		// �ð� �ݴ�������� �׸�������

	}
	// Rasterizer State ����
	{
		HRESULT hr = Device->CreateRasterizerState
		(
			&desc,
			&rasterState
		);

		assert(SUCCEEDED(hr));
	}
}