#include "framework.h"
#include "Texture.h"

//////////////////////////////////////////////////////////////////////////////
//
//	Imagefile Type:
//	case 1) Marco.png �ϳ��� ����Ұ� ���ִ� ���
//	case 2) idle(1).png, idle(2).png ��Ұ� ���������ִ� ���
//	case 3)	idle.png, work.png, attack.png ���ۺ��� ��Ұ� ���������ִ� ���
//
//	Animation :
//	-> Animationclip( IDLE )
//	-> AnimationClip( WALK )
//	
//	Texture -> Player Control
//
//////////////////////////////////////////////////////////////////////////////

Texture::Texture(wstring imageFile, wstring shaderFile)
{
	shader = new TextureShader(shaderFile);
	CreateShaderResourceView(imageFile);
	CreateVertexBuffer(0, 0, 0, 0);
	// ��� ������ �׸���.
}

Texture::Texture(wstring imageFile, wstring shaderFile, float startX, float startY, float endX, float endY)
{
	shader = new TextureShader(shaderFile);
	CreateShaderResourceView(imageFile);
	CreateVertexBuffer(startX, startY, endX, endY);
	// ������ ��������� �׸���.
}

Texture::~Texture()
{
	for (auto v : spriteResources)
	{
		SAFE_RELEASE(v.second->srv);
		SAFE_DELETE(v.second);
	}
	spriteResources.clear();

	SAFE_RELEASE(vertexBuffer);
	SAFE_DELETE(shader);
}

void Texture::Update(Matrix V, Matrix P)
{
	if (!active)
		return;

	Matrix W, S, T, R;

	D3DXMatrixIdentity(&S);
	D3DXMatrixIdentity(&T);
	D3DXMatrixIdentity(&R);

	D3DXMatrixTranslation(&T, position.x, position.y, 0.0f);

	D3DXMatrixScaling
	(
		&S,
		(scale.x * spriteSize.x),
		(scale.y * spriteSize.y),
		0.0f
	);

	D3DXMatrixRotationYawPitchRoll
	(
		&R,
		rotation.y * (float)(3.141592 / 180.0f),
		rotation.x * (float)(3.141592 / 180.0f),
		rotation.z * (float)(3.141592 / 180.0f)
	);
	// Y = flip, Z = 2�������� ���� angle

	W = S * R * T;
	// World �� Scalse x Trans x Rotate

	shader->Update(W, V, P);
	srv = FindShaderResourceView();
	imageSize = FindSpriteSize();

	srv = SRVMANAGER->FindShaderResourceView(imageFile);
	imageSize = SRVMANAGER->FindSpriteSize(imageFile);

	UpdateBlock();

	// ImageFile ���� ����Ǵ� ��� -> srv �� �ٲ�����


	shader->SetShaderResourceView(srv);
	// Sprite�� ���� Size ����, ������� W �� V, P �� �Բ� Shader �� ������.
	// next : UpdateBlock, ShaderResource change �ϴ°�
}

void Texture::Render()
{
	if (!active)
		return;

	UINT stride = sizeof(Vertex);	// ���� ũ�⿡���� ����
	UINT offset = 0;

	// IA �ܰ� Input Assembly

	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->IASetInputLayout(shader->GetLayout());
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ������ �ܰ�� Shader ���� �����Ѵ�.

	shader->Draw(6, 0);
}

void Texture::UpdateBlock()
{
	// Vertices ����
	Vertex vertices[6];

	// Vertices ����
	{
		vertices[0].position = Vector3(-0.5f, -0.5f, 0.0f); // 0
		vertices[1].position = Vector3(-0.5f, +0.5f, 0.0f);	// 1
		vertices[2].position = Vector3(+0.5f, -0.5f, 0.0f);	// 3
		vertices[3].position = Vector3(+0.5f, -0.5f, 0.0f);	// 3
		vertices[4].position = Vector3(-0.5f, +0.5f, 0.0f);	// 1
		vertices[5].position = Vector3(+0.5f, +0.5f, 0.0f);	// 2

		float startX = spriteOffset.x / imageSize.x;
		float startY = spriteOffset.y / imageSize.y;
		float endX = (spriteOffset.x + spriteSize.x) / imageSize.x;
		float endY = (spriteOffset.y + spriteSize.y) / imageSize.y;

		vertices[0].uv = Vector2(startX, endY);
		vertices[1].uv = Vector2(startX, startY);
		vertices[2].uv = Vector2(endX, endY);
		vertices[3].uv = Vector2(endX, endY);
		vertices[4].uv = Vector2(startX, startY);
		vertices[5].uv = Vector2(endX, startY);
	}

	// Map �� UnMap ���̿��� �����͸� �����Ѵ�.
	D3D11_MAPPED_SUBRESOURCE subResource;
	DeviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	// Subresource �� �����͸� �����غ��Ѵ�.

	memcpy(subResource.pData, vertices, sizeof(Vertex) * 6);
	// Subresource �� ������ ���� �����͸� �־���´�.

	DeviceContext->Unmap(vertexBuffer, 0);
	// �ٽ� Unmap �� ���༭ ���´�.
}

void Texture::UpdateColorBuffer(Color color, int path, float time, float time2, float time3)
{
	shader->UpdateColorBuffer(color, path, time, time2, time3);
}

////////////////////////////////////////////////////////////
//
//
//					  1 	|      2
//	            --------------------------
//					  0		|      3
//
//
////////////////////////////////////////////////////////////

void Texture::CreateVertexBuffer(float startX, float startY, float endX, float endY)
{
	// Vertices ����
	Vertex vertices[6];

	// Vertices ����
	{
		vertices[0].position = Vector3(-0.5f, -0.5f, 0.0f); // 0
		vertices[1].position = Vector3(-0.5f, +0.5f, 0.0f);	// 1
		vertices[2].position = Vector3(+0.5f, -0.5f, 0.0f);	// 3
		vertices[3].position = Vector3(+0.5f, -0.5f, 0.0f);	// 3
		vertices[4].position = Vector3(-0.5f, +0.5f, 0.0f);	// 1
		vertices[5].position = Vector3(+0.5f, +0.5f, 0.0f);	// 2

		// startX, endY �� NCD �� �����Ѵ�.
		{
			// ������ �Է� �ޱ�

			D3DX11_IMAGE_INFO imageInfo;

			HRESULT hr = D3DX11GetImageInfoFromFile
			(
				imageFile.c_str(),
				nullptr,
				&imageInfo,
				nullptr
			);

			float textureWidth = (float)imageInfo.Width;
			float textureHeight = (float)imageInfo.Height;

			spriteOffset = Vector2(startX, startY);
			spriteSize = Vector2(endX - startX, endY - startY);
			imageSize = Vector2(textureWidth, textureHeight);

			// �������

			startX = (startX > 0) ? startX / textureWidth : 0.0f;
			startY = (startY > 0) ? startY / textureHeight : 0.0f;

			endX = (endX > 0) ? endX / textureWidth : 1.0f;
			endY = (endY > 0) ? endY / textureHeight : 1.0f;

			if ((endX - startX) == 1.0f)
				spriteSize.x = textureWidth;

			if ((endY - startY) == 1.0f)
				spriteSize.y = textureHeight;
			// FULL

			// startX, startY -> NCD ��ǥ�� ��ȯ �Ϸ�

			vertices[0].uv = Vector2(startX, endY);
			vertices[1].uv = Vector2(startX, startY);
			vertices[2].uv = Vector2(endX, endY);
			vertices[3].uv = Vector2(endX, endY);
			vertices[4].uv = Vector2(startX, startY);
			vertices[5].uv = Vector2(endX, startY);
			// ���� + : startX, endY  |  - : endX, startY
		}
	}


	// Buffer �� ����
	{
		// desc ����
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(Vertex) * 6;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		// data ����
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

void Texture::CreateIndexBuffer()
{
}

//////////////////////////////////////////////////////////
//
//	ImageFile �� �а� �����Ѵ�.
//
//////////////////////////////////////////////////////////

void Texture::CreateShaderResourceView(wstring imageFile)
{
	this->imageFile = imageFile;


	ID3D11ShaderResourceView* buf = SRVMANAGER->FindShaderResourceView(imageFile);

	if (buf != nullptr)
		srv = buf;
	else
		srv = SRVMANAGER->CreateShaderResourceView(imageFile);


	return;

	string str;
	str.assign(imageFile.begin(), imageFile.end());

	if (_access(str.c_str(), 00) != 0)
	{
		str += " Texture::CreateShaderResourceView : File not found ";
		MessageBoxA(NULL, str.c_str(), "Game", MB_OK);
		return;
	}

	// Shader Resource View ���� 3���� Style �� �ִ�.
	// memory,
	// stream(dds), DX ���� ���� ���� Ȯ�����̸� �ٸ� �Լ����� ����Ѵ�.
	// file(.png) �Ϲ����� �̹��� ������ ����Ѵ�.

	if (spriteResources.find(imageFile) != spriteResources.end())
		return;	// �̹� ��� �Ǿ����� ��쿡 ���� ���� ó��

	SpriteResource* spriteResource = new SpriteResource();

	// Image ���� ��������

	HRESULT hr = D3DX11CreateShaderResourceViewFromFile
	(
		Device,
		imageFile.c_str(),
		NULL,
		NULL,
		&spriteResource->srv,
		nullptr
	);

	assert(SUCCEEDED(hr));

	// Image size �Է�

	// ImageFile �� ���� info �� �����´�.
	D3DX11_IMAGE_INFO imageInfo;

	hr = D3DX11GetImageInfoFromFile
	(
		imageFile.c_str(),
		nullptr,
		&imageInfo,
		nullptr
	);

	assert(SUCCEEDED(hr));

	spriteResource->spriteSize = Vector2((float)imageInfo.Width, (float)imageInfo.Height);

	// srv �� ���� �ִ� �� ����ֱ�
	srv = spriteResource->srv;
	shader->SetShaderResourceView(srv);
	spriteResources.insert(make_pair(imageFile, spriteResource));
}

Vector2 Texture::GetTextureRealSize()
{
	Vector2 size;
	size.x = scale.x * spriteSize.x;
	size.y = scale.y * spriteSize.y;

	return size;
}

Vector2 Texture::FindSpriteSize()
{
	if (spriteResources.find(imageFile) != spriteResources.end())
	{
		auto find = spriteResources.find(imageFile);
		return find->second->spriteSize;
		// �̹� ��� �Ǿ����� ��쿡 ���� ���� ó��
	}

	return Vector2(1.0f, 1.0f);
}

ID3D11ShaderResourceView* Texture::FindShaderResourceView()
{
	if (spriteResources.find(imageFile) != spriteResources.end())
	{
		auto find = spriteResources.find(imageFile);
		return find->second-> srv;
		// �̹� ��� �Ǿ����� ��쿡 ���� ���� ó��
	}

	return nullptr;
}