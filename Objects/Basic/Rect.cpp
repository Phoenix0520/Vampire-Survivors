#include "framework.h"
#include "Rect.h"

//////////////////////////////////////////////////////////////////////
// 생성자, 소멸자
//
// 생성자 : shader 동적할당 -> CreateVertexBuffer -> CreateIndexBuffer
// 소멸자 : 역순으로 해제
//
/////////////////////////////////////////////////////////////////////
Rect::Rect(wstring shaderFile)
{
	shader = new ColorShader(shaderFile);
	CreateVertexBuffer();
	CreateIndexBuffer();
}

Rect::~Rect()
{
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(vertexBuffer);
	SAFE_DELETE(shader);
}

void Rect::Update(Matrix V, Matrix P)
{
	Matrix W, S, T, R;

	D3DXMatrixIdentity(&S);
	D3DXMatrixIdentity(&T);
	D3DXMatrixIdentity(&R);


	D3DXMatrixTranslation(&T, position.x, position.y, 0.0f);
	D3DXMatrixScaling(&S, scale.x, scale.y, 0.0f);
	D3DXMatrixRotationYawPitchRoll
	(
		&R,
		(float)D3DXToRadian(rotation.x),
		(float)D3DXToRadian(rotation.y),
		(float)D3DXToRadian(rotation.z)
	);

	W = S * T * R;
	// World 는 Scalse x Trans

	shader->Update(W, V, P);
	// 만들어준 W 를 V, P 와 함께 Shader 에 보낸다.
}

void Rect::Render()
{
	UINT stride = sizeof(Vertex);	// 정점 크기에대한 정보
	UINT offset = 0;

	// IA 단계 Input Assembly

	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	DeviceContext->IASetInputLayout(shader->GetLayout());
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 나머지 단계는 Shader 에서 진행한다.

	shader->DrawIndexed(6, 0, 0);
}

void Rect::UpdateBlock(Color color)
{
	Vertex* vertices = new Vertex[4];

	// Vertices 색 변경
	{
		vertices[0].position = Vector3(-0.5f, -0.5f, 0.0f);
		vertices[1].position = Vector3(-0.5f, 0.5f, 0.0f);
		vertices[2].position = Vector3(0.5f, 0.5f, 0.0f);
		vertices[3].position = Vector3(0.5f, -0.5f, 0.0f);

		vertices[0].color = color;
		vertices[1].color = color;
		vertices[2].color = color;
		vertices[3].color = color;
	}

	// Map 과 UnMap 사이에서 데이터를 변경한다. CPU 쓰기가 허용된 경우에만 가능하다.

	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	
	DeviceContext->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);

	// 데이터를 변경
	memcpy(mapped_subresource.pData, vertices, sizeof(Vertex) * 4);

	DeviceContext->Unmap(this->vertexBuffer, 0);
}

////////////////////////////////////////////////////////////////////
//
//
//
//						1		|		2
//					-------------------------
//						0		|		3
//
//
//
////////////////////////////////////////////////////////////////////

void Rect::CreateVertexBuffer()
{
	Vertex* vertices = new Vertex[4];

	// Vertices 정점 자원
	{
		vertices[0].position = Vector3(-0.5f, -0.5f, 0.0f);
		vertices[1].position = Vector3(-0.5f, 0.5f, 0.0f);
		vertices[2].position = Vector3(0.5f, 0.5f, 0.0f);
		vertices[3].position = Vector3(0.5f, -0.5f, 0.0f);

		vertices[0].color = Color(1.0f, 0.0f, 0.0f, 1.0f);
		vertices[1].color = Color(1.0f, 0.0f, 0.0f, 1.0f);
		vertices[2].color = Color(1.0f, 0.0f, 0.0f, 1.0f);
		vertices[3].color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	}

	// Buffer 로 연결
	{
		// desc 설정
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(Vertex) * 4;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		// data 설정
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));

		data.pSysMem = vertices;

		HRESULT hr = Device->CreateBuffer
		(
			&desc,
			&data,
			&vertexBuffer
		);

		assert(SUCCEEDED(hr));
	}
}

void Rect::CreateIndexBuffer()
{
	UINT indecies[] = { 0, 1, 2, 3, 0, 2 };

	// Buffer 로 연결
	{
		// desc 설정
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(UINT) * 6;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		// data 설정
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));

		data.pSysMem = indecies;

		HRESULT hr = Device->CreateBuffer
		(
			&desc,
			&data,
			&indexBuffer
		);

		assert(SUCCEEDED(hr));
	}
}