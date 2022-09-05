#include "framework.h"
#include "Line.h"

///////////////////////////////////////////////////////////////
// ������ , �Ҹ���
//
// ������ : shader �� �����Ҵ����ص� -> CreateVertexBuffer
// �Ҹ��� : ������ �������� �������ش�.
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
	// World �� Scalse x Trans

	shader->Update(W, V, P);
	// ������� W �� V, P �� �Բ� Shader �� ������.
}

void Line::Render()
{
	UINT stride = sizeof(Vertex);	// ���� ũ�⿡���� ����
	UINT offset = 0;

	// IA �ܰ� Input Assembly

	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->IASetInputLayout(shader->GetLayout());
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// ������ �ܰ�� Shader ���� �����Ѵ�.

	shader->Draw(4, 0);
}

void Line::UpdateBlock()
{
}

void Line::CreateVertexBuffer()
{
	Vertex* vertices = new Vertex[4];

	// Vertices ���� �ڿ�
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

	// Buffer �� ����
	{
		// desc ����
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(Vertex) * 4;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		// data ����
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
