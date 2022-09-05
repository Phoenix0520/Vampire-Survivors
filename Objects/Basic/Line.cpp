#include "framework.h"
#include "Line.h"

///////////////////////////////////////////////////////////////
// 생성자 , 소멸자
//
// 생성자 : shader 를 동적할당해준뒤 -> CreateVertexBuffer
// 소멸자 : 생성한 역순으로 해제해준다.
///////////////////////////////////////////////////////////////

Line::Line(wstring shaderFile)
{
	shader = new ColorShader(shaderFile);
	CreateVertexBuffer();
}

Line::~Line()
{
	SAFE_RELEASE(vertexBuffer)
		SAFE_DELETE_ARRAY(shader);
}

void Line::Update(Matrix V, Matrix P)
{
	Matrix W, S, T, R;

	D3DXMatrixIdentity(&S);
	D3DXMatrixIdentity(&T);
	D3DXMatrixIdentity(&R);

	D3DXMatrixTranslation(&T, position.x, position.y, 0.0f);
	D3DXMatrixScaling(&S, scale.x, scale.y, 0.0f);

	W = S * T;
	// World 는 Scalse x Trans

	shader->Update(W, V, P);
	// 만들어준 W 를 V, P 와 함께 Shader 에 보낸다.
}

void Line::Render()
{
	UINT stride = sizeof(Vertex);	// 정점 크기에대한 정보
	UINT offset = 0;

	// IA 단계 Input Assembly

	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->IASetInputLayout(shader->GetLayout());
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// 나머지 단계는 Shader 에서 진행한다.

	shader->Draw(4, 0);
}

void Line::UpdateBlock()
{
}

void Line::CreateVertexBuffer()
{
	Vertex* vertices = new Vertex[4];

	// Vertices 정점 자원
	{
		vertices[0].position = Vector3(-0.5f, -0.5f, 0.0f);
		vertices[1].position = Vector3(-0.5f, 0.5f, 0.0f);
		vertices[2].position = Vector3(0.5f, 0.5f, 0.0f);
		vertices[3].position = Vector3(0.5f, -0.5f, 0.0f);

		vertices[0].color = Color(1.0f, 0.0f, 0.0f, 1.0f);
		vertices[1].color = Color(0.0f, 1.0f, 0.0f, 1.0f);
		vertices[2].color = Color(0.0f, 0.0f, 1.0f, 1.0f);
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
		D3D11_SUBRESOURCE_DATA	data;
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
