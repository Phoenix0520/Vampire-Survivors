#include "framework.h"
#include "TextureShader.h"

TextureShader::TextureShader(wstring shaderFile)
{
	CreateShader(shaderFile);
	CreateInputLayout();	// blob 생성
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
//	ColorShader 에서의 내용과 비슷하다.
//
//	
//	Create Shader 를 통해 Blob 생성부터 시작
//  -> 생성자에 있는 순서대로 실행
//
//	Update : Map UnMap 사이에서 텍스쳐 변경내용 적용
//
//
///////////////////////////////////////////////////////////////////////////////////

void TextureShader::Update(Matrix W, Matrix V, Matrix P)
{
	TRANSFORM_DATA temp;

	D3DXMatrixTranspose(&temp.proj, &P);
	D3DXMatrixTranspose(&temp.view, &V);
	D3DXMatrixTranspose(&temp.world, &W);

	// 바꿔준 데이터를 적용시킨다.

	// Buffer 를 수정하려면 Map 과 UnMap 사이를 수정해야한다.
	// 그래서 D3D11_MAPPED_SUBRESOURCE 함수를 사용해서 수정한다.

	// Buffer를 수정하려면  Map - UnMap()

	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	DeviceContext->Map(this->matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);

	// 데이터를 변경
	memcpy_s(mapped_subresource.pData, mapped_subresource.RowPitch, &temp, sizeof(temp));

	DeviceContext->Unmap(this->matrixBuffer, 0);

}

void TextureShader::UpdateColorBuffer(Color color, int path, float time, float time2, float time3)
{
	// Color Data
	
	COLOR_DATA temp;

	// 바꿔줄 인자로 데이터를 적용시킨다.
	temp.color	= color;
	temp.path	= path;
	temp.time	= time;
	temp.dummy1 = time2;
	temp.dummy2 = time3;

	// 데이터를 변경
	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	DeviceContext->Map(this->colorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
	memcpy_s(mapped_subresource.pData, mapped_subresource.RowPitch, &temp, sizeof(temp));
	DeviceContext->Unmap(this->colorBuffer, 0);

	// Pipeline 에 추가
}

void TextureShader::Draw(UINT vertexCount, UINT startVertexLocation)
{
	// IA 는 Object Class 에서 해결

	// VS 단계 Vertex Shader

	DeviceContext->VSSetShader(vertexShader, nullptr, 0);
	DeviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	// RS 단계 Raster Stage

	DeviceContext->RSSetState(rasterState);

	// PS 단계 Pixel Shader

	DeviceContext->PSSetShader(pixelShader, nullptr, 0);
	DeviceContext->PSSetShaderResources(0, 1, &srv);
	DeviceContext->PSSetSamplers(0, 1, &samplerState);
	DeviceContext->PSSetConstantBuffers(1, 1, &colorBuffer);
	// srv setting 및 sampling

	// OM 단계 Output Merger

	DeviceContext->OMSetBlendState(blendState, nullptr, 0xffffffff);
	// AlphaBlending

	// Draw 를 통해 최종적으로 그리면 끝난다. -> 파이프라인 종료

	DeviceContext->Draw(vertexCount, startVertexLocation);
	// 정점의 갯수, 시작점을 입력해서 Draw 를 해준다.
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

	// 정점 셰이더 연결
	{
		HRESULT hr = D3DX11CompileFromFile
		(
			shaderFile.c_str(),		// 파일명
			0,					// MACRO 사용안함
			0,					// Include 사용안함
			"VS",				// hlsl 파일에 정의된 함수명과 동일
			"vs_5_0",			// 사용된 셰이더 버전
			0,					// flag1
			0,					// flag2
			0,			// thread pump
			&vsBlob,			// blob
			0,			// 에러메세지
			0				// 에러메세지 받는주소
		);

		assert(SUCCEEDED(hr));

		hr = Device->CreateVertexShader
		(
			vsBlob->GetBufferPointer(),		// Blob 의 시작 주소
			vsBlob->GetBufferSize(),		// Blob 의 크기
			nullptr,						//
			&vertexShader					// VS 의 주소
		);

		assert(SUCCEEDED(hr));
	}

	// 픽셀 셰이더 연결
	{
		HRESULT hr = D3DX11CompileFromFile
		(
			shaderFile.c_str(),		// 파일명
			0,					// MACRO 사용안함
			0,					// Include 사용안함
			"PS",				// hlsl 파일에 정의된 함수명과 동일
			"ps_5_0",			// 사용된 셰이더 버전
			0,					// flag1
			0,					// flag2
			nullptr,			// thread pump
			&psBlob,			// blob
			nullptr,			// 에러메세지
			nullptr				// 에러메세지 받는주소
		);

		assert(SUCCEEDED(hr));

		hr = Device->CreatePixelShader
		(
			psBlob->GetBufferPointer(),		// Blob 의 시작 주소
			psBlob->GetBufferSize(),		// Blob 의 크기
			nullptr,						//
			&pixelShader					// PS 의 주소
		);

		assert(SUCCEEDED(hr));
	}
}

void TextureShader::CreateConstantBuffer()
{
	// hlsl 파일에 TransformBuffer 가 들어가게된다.
	// Constant Buffer, Matrix Buffer 다 똑같은 Buffer 이다.

	// Buffer 로 연결
	{
		// desc 설정
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DYNAMIC;			// Data 의 용도 : CPU Write & GPU Read. GPU 에서 변경이 가능하다.
		desc.ByteWidth = sizeof(TRANSFORM_DATA);		// Data 갯수 입력
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// Buffer 의 타입
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// Buffer 의 CPU 의 역할

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
	// Buffer 로 연결
	{
		// desc 설정
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DYNAMIC;				// Data 의 용도 : CPU Write & GPU Read. GPU 에서 변경이 가능하다.
		desc.ByteWidth = sizeof(COLOR_DATA);			// Data 갯수 입력
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// Buffer 의 타입
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// Buffer 의 CPU 의 역할

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
	// 레이아웃 생성
	{
		D3D11_INPUT_ELEMENT_DESC layout[2];

		// Struct Vector3
		{
			layout[0].SemanticName = "POSITION";					// 시멘틱스 네임
			layout[0].SemanticIndex = 0;							// 시멘틱스 넘버
			layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;	// 포맷
			layout[0].InputSlot = 0;
			layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// 정점데이터가된다.
			layout[0].AlignedByteOffset = 0;
			layout[0].InstanceDataStepRate = 0;
		}

		// Struct Color
		{
			layout[1].SemanticName = "TEXCOORD";					// 시멘틱스 네임
			layout[1].SemanticIndex = 0;							// 시멘틱스 넘버
			layout[1].Format = DXGI_FORMAT_R32G32_FLOAT;	// 포맷
			layout[1].InputSlot = 0;
			layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// 정점데이터가된다.
			layout[1].AlignedByteOffset = 12;							// float2 * 3 ( x, y, x) + w (동차)
			layout[1].InstanceDataStepRate = 0;
		}

		// 정점이 어떻게 그려질지를 정한다.
		HRESULT hr = Device->CreateInputLayout
		(
			layout,						// desc name
			2,							// desc 갯수
			vsBlob->GetBufferPointer(), // 정점버퍼의 주소를 준다.
			vsBlob->GetBufferSize(),	// 버퍼의 크기
			&inputLayout
		);

		assert(SUCCEEDED(hr));
	}
}

void TextureShader::CreateSampleState()
{
	// desc 생성
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));

	// desc 설정
	{
		// UV 좌표와 W 좌표
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		// 정규화된 좌표를 만들었을때 어떻게 작동할 것인지에 대한 플래그
		// WRAP = 원본픽셀로 나머지 부분을 채운다는 의미이다.
		// CLAMP = 마지막 픽셀로 채운다.
		// MIRROR = 인접한 픽셀을 반전시켜서 채운다.

		desc.BorderColor[0] = 1;
		desc.BorderColor[1] = 0;
		desc.BorderColor[2] = 0;
		desc.BorderColor[3] = 1;
		// 외곽선을 나타낸다.

		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		// 이전 현재 데이터를 비교하는 방법을 정하는 플래그
		// 기존 샘플과 4개의 값을 비교하여 통과시킬지 아닐지를 결정한다.

		desc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		// Scaling 축소 및 확대 가 될때 평균값을 내서 미리 1/4 크기로 이미지를 구성한다.
		// MIN = 이미지 축소
		// MAG = 이미지 확대
		// POINT = 두개의 픽셀이 존재한다면 한픽셀은 폐기한다.
		// LINEAR = 두개의 픽셀의 평균값

		desc.MaxAnisotropy = 16;
		// 비등방성 필터링 (이방성 필터링)

		desc.MaxLOD = FLT_MAX;
		desc.MinLOD = FLT_MIN;
		// 거리에 따른 이미지 선명도

		desc.MipLODBias = 0.0f;
	}

	// Sampler State 생성
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
	// Alpha 값을 책정한다.

	// desc 생성
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));

	// desc 설정
	{
		// Blending 은 OM 단계에서 설정된다.
		desc.AlphaToCoverageEnable = false;						// 뒤의 테두리 제거
		desc.IndependentBlendEnable = true;							// 알파 혼합

		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;		// 0.9 라고하면
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	// 0.1 이되는것이다.
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		// Blending 의 표준 Flags

	}

	// BlendState 생성
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
	// desc 생성
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

	// desc 설정
	{
		desc.FillMode = D3D11_FILL_SOLID;
		// 내부를 칠하는 것
		desc.CullMode = D3D11_CULL_NONE;
		// 뒷부분의 색을 칠할것인지 양면 모두 무조건 보이게 하겠다는 의미다.
		desc.FrontCounterClockwise = false;
		// 시계 반대방향으로 그릴것인지

	}
	// Rasterizer State 생성
	{
		HRESULT hr = Device->CreateRasterizerState
		(
			&desc,
			&rasterState
		);

		assert(SUCCEEDED(hr));
	}
}