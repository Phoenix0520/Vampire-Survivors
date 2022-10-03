#include  "framework.h"
#include  "Collider.h"
#include  "ColorShader.h"

#define PI 3.141592f

Collider::Collider()
{
	wstring strShader = L"./Shader/HLSL/Color.hlsl";

	shader = new ColorShader(strShader);
	D3DXMatrixIdentity(&world);
	CreateBuffer();
}

Collider::~Collider()
{
	SAFE_DELETE(shader);
	SAFE_RELEASE(vertexBuffer);
}

void Collider::Update(Matrix V, Matrix P)
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
		rotation.y * PI / 180.0f,
		rotation.x * PI / 180.0f,
		rotation.z * PI / 180.0f
	);

	W = S * R *  T;
	world = S * R * T;

	shader->Update(W, V, P);
}

void Collider::Render()
{
	UINT  stride = sizeof(Vertex);
	UINT  offset = 0;

	if (visibleCollider)
		return;

	// IA단계  Input Assembly
	DeviceContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &offset);
	DeviceContext->IASetInputLayout(shader->GetLayout());
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// VS-OM
	shader->Draw(5, 0);
}

bool Collider::InterSect(Collider * A, Collider * B)
{
	Matrix world1 = A->GetWorld();
	Matrix world2 = B->GetWorld();

	float halfWidth = fabs(world1._11 *0.5f);
	float halfHeight = fabs(world1._22 *0.5f);
	float leftA = world1._41 - halfWidth;
	float rightA = world1._41 + halfWidth;
	float topA = world1._42 + halfHeight;
	float bottomA = world1._42 - halfHeight;

	halfWidth = fabs(world2._11 *0.5f);
	halfHeight = fabs(world2._22 *0.5f);

	float leftB = world2._41 - halfWidth;
	float rightB = world2._41 + halfWidth;
	float topB = world2._42 + halfHeight;
	float bottomB = world2._42 - halfHeight;

	// 충돌 Check
	if (rightA < leftB || leftA > rightB)
		return false;
	if (topA < bottomB || bottomA > topB)
		return false;

	return true;
}

void Collider::CreateBuffer()
{
	Vertex vertices[5];

	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);  // 0
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);  // 1
	vertices[2].Position = Vector3(+0.5f, +0.5f, 0.0f);  // 3
	vertices[3].Position = Vector3(+0.5f, -0.5f, 0.0f);  // 2
	vertices[4].Position = Vector3(-0.5f, -0.5f, 0.0f);  // 0

	vertices[0].Color = Color(0.0f, 1.0f, 0.0f, 0.7f);
	vertices[1].Color = Color(0.0f, 1.0f, 0.0f, 0.7f);
	vertices[2].Color = Color(0.0f, 1.0f, 0.0f, 0.7f);
	vertices[3].Color = Color(0.0f, 1.0f, 0.0f, 0.7f);
	vertices[4].Color = Color(0.0f, 1.0f, 0.0f, 0.7f);

	{
		D3D11_BUFFER_DESC  desc;

		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.ByteWidth = sizeof(Vertex) * 5;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = vertices;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}
}