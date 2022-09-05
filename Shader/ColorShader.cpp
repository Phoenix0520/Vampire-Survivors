#include "framework.h"
#include "ColorShader.h"

///////////////////////////////////////////////////////////////
// ������ , �Ҹ���
//
// ������ : CreateShader -> InputLayout -> CreateMatrixBuffer
// �Ҹ��� : ������ �������� �������ش�.
///////////////////////////////////////////////////////////////
ColorShader::ColorShader(wstring shaderFile)
{
	CreateShader(shaderFile);
	CreateInputLayout();
	CreateMatrixBuffer();
}

ColorShader::~ColorShader()
{
	SAFE_RELEASE(matrixBuffer)
	SAFE_RELEASE(inputLayout)
	SAFE_RELEASE(psBlob)
	SAFE_RELEASE(pixelShader)
	SAFE_RELEASE(vsBlob)
	SAFE_RELEASE(vertexShader)
}

///////////////////////////////////////////////////////////////
// Matrix �� ���õ� �����͸� Shader �� �����ֱ� ���� �غ��۾�
// Update : MatrixTranspose
//
///////////////////////////////////////////////////////////////
void ColorShader::Update(Matrix W, Matrix V, Matrix P)
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

void ColorShader::Draw(UINT VertexCount, UINT StartVertexLocation)
{
	// IA �� ���� Class ���� �ذ�

	// VS �ܰ� Vertex Shader

	DeviceContext->VSSetShader(vertexShader, nullptr, 0);
	DeviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	// RS �ܰ� Raster Stage

	// �ϴ��� ������ ������ �н�

	// PS �ܰ� Pixel Shader
	DeviceContext->PSSetShader(pixelShader, nullptr, 0);

	// OM �ܰ� Output Merger

	// ������ ������ ������ �н�

	// Draw �� ���� ���������� �׸��� ������. -> ���������� ����
	DeviceContext->Draw(VertexCount, StartVertexLocation);
	// ������ ����, �������� �Է��ؼ� Draw �� ���ش�.
}

void ColorShader::DrawIndexed(UINT IndexCount, UINT StartVertexLocation, int BaseVertexLocation)
{
	// IA �� ���� Class ���� �ذ�

	// VS �ܰ� Vertex Shader

	DeviceContext->VSSetShader(vertexShader, nullptr, 0);
	DeviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	// RS �ܰ� Raster Stage

	// �ϴ��� ������ ������ �н�

	// PS �ܰ� Pixel Shader
	DeviceContext->PSSetShader(pixelShader, nullptr, 0);

	// OM �ܰ� Output Merger

	// ������ ������ ������ �н�

	// Draw �� ���� ���������� �׸��� ������. -> ���������� ����
	DeviceContext->DrawIndexed(IndexCount, StartVertexLocation, BaseVertexLocation);
	// ������ ����, �������� �Է��ؼ� Draw �� ���ش�.
}

void ColorShader::CreateShader(wstring shaderFile)
{
	string str;
	str.assign(shaderFile.begin(), shaderFile.end()); // wstring to string

	if (_access(str.c_str(), 00) != 0)
	{
		str = "File not found " + str;
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

void ColorShader::CreateInputLayout()
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
			layout[1].SemanticName = "COLOR";					// �ø�ƽ�� ����
			layout[1].SemanticIndex = 0;							// �ø�ƽ�� �ѹ�
			layout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;	// ����
			layout[1].InputSlot = 0;
			layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// ���������Ͱ��ȴ�.
			layout[1].AlignedByteOffset = 12;							// float4 * 3 ( x, y, x) + w (����)
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

void ColorShader::CreateMatrixBuffer()
{
	// hlsl ���Ͽ� TransformBuffer �� ���Եȴ�.

	// Buffer �� ����
	{
		// desc ����
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DYNAMIC;				// Data �� �뵵 : CPU Write & GPU Read. GPU ���� ������ �����ϴ�.
		desc.ByteWidth = sizeof(TRANSFORM_DATA);		// Data ���� �Է�
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// Buffer �� Ÿ��
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// Buffer �� CPU �� ����

		HRESULT hr = Device->CreateBuffer
		(
			&desc,
			nullptr,
			&matrixBuffer
		);

		assert(SUCCEEDED(hr));
	}
}