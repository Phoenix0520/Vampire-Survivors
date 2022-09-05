#include "framework.h"
#include "ColorShader.h"

///////////////////////////////////////////////////////////////
// 생성자 , 소멸자
//
// 생성자 : CreateShader -> InputLayout -> CreateMatrixBuffer
// 소멸자 : 생성한 역순으로 해제해준다.
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
// Matrix 에 관련된 데이터를 Shader 에 보내주기 위한 준비작업
// Update : MatrixTranspose
//
///////////////////////////////////////////////////////////////
void ColorShader::Update(Matrix W, Matrix V, Matrix P)
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

void ColorShader::Draw(UINT VertexCount, UINT StartVertexLocation)
{
	// IA 는 도형 Class 에서 해결

	// VS 단계 Vertex Shader

	DeviceContext->VSSetShader(vertexShader, nullptr, 0);
	DeviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	// RS 단계 Raster Stage

	// 일단은 내용이 없으니 패스

	// PS 단계 Pixel Shader
	DeviceContext->PSSetShader(pixelShader, nullptr, 0);

	// OM 단계 Output Merger

	// 별도의 내용은 없으니 패스

	// Draw 를 통해 최종적으로 그리면 끝난다. -> 파이프라인 종료
	DeviceContext->Draw(VertexCount, StartVertexLocation);
	// 정점의 갯수, 시작점을 입력해서 Draw 를 해준다.
}

void ColorShader::DrawIndexed(UINT IndexCount, UINT StartVertexLocation, int BaseVertexLocation)
{
	// IA 는 도형 Class 에서 해결

	// VS 단계 Vertex Shader

	DeviceContext->VSSetShader(vertexShader, nullptr, 0);
	DeviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	// RS 단계 Raster Stage

	// 일단은 내용이 없으니 패스

	// PS 단계 Pixel Shader
	DeviceContext->PSSetShader(pixelShader, nullptr, 0);

	// OM 단계 Output Merger

	// 별도의 내용은 없으니 패스

	// Draw 를 통해 최종적으로 그리면 끝난다. -> 파이프라인 종료
	DeviceContext->DrawIndexed(IndexCount, StartVertexLocation, BaseVertexLocation);
	// 정점의 갯수, 시작점을 입력해서 Draw 를 해준다.
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

void ColorShader::CreateInputLayout()
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
			layout[1].SemanticName = "COLOR";					// 시멘틱스 네임
			layout[1].SemanticIndex = 0;							// 시멘틱스 넘버
			layout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;	// 포맷
			layout[1].InputSlot = 0;
			layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// 정점데이터가된다.
			layout[1].AlignedByteOffset = 12;							// float4 * 3 ( x, y, x) + w (동차)
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

void ColorShader::CreateMatrixBuffer()
{
	// hlsl 파일에 TransformBuffer 가 들어가게된다.

	// Buffer 로 연결
	{
		// desc 설정
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DYNAMIC;				// Data 의 용도 : CPU Write & GPU Read. GPU 에서 변경이 가능하다.
		desc.ByteWidth = sizeof(TRANSFORM_DATA);		// Data 갯수 입력
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// Buffer 의 타입
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// Buffer 의 CPU 의 역할

		HRESULT hr = Device->CreateBuffer
		(
			&desc,
			nullptr,
			&matrixBuffer
		);

		assert(SUCCEEDED(hr));
	}
}