#include "framework.h"
#include "Texture.h"

//////////////////////////////////////////////////////////////////////////////
//
//	Imagefile Type:
//	case 1) Marco.png 하나의 모든요소가 다있는 경우
//	case 2) idle(1).png, idle(2).png 요소가 나누어져있는 경우
//	case 3)	idle.png, work.png, attack.png 동작별로 요소가 나누어져있는 경우
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
	// 모든 내용을 그린다.
}

Texture::Texture(wstring imageFile, wstring shaderFile, float startX, float startY, float endX, float endY)
{
	shader = new TextureShader(shaderFile);
	CreateShaderResourceView(imageFile);
	CreateVertexBuffer(startX, startY, endX, endY);
	// 설정한 내용까지만 그린다.
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
	// Y = flip, Z = 2차원에서 보는 angle

	W = S * R * T;
	// World 는 Scalse x Trans x Rotate

	shader->Update(W, V, P);
	srv = FindShaderResourceView();
	imageSize = FindSpriteSize();

	srv = SRVMANAGER->FindShaderResourceView(imageFile);
	imageSize = SRVMANAGER->FindSpriteSize(imageFile);

	UpdateBlock();

	// ImageFile 명이 변경되는 경우 -> srv 가 바껴야함


	shader->SetShaderResourceView(srv);
	// Sprite에 대한 Size 변경, 만들어준 W 를 V, P 와 함께 Shader 에 보낸다.
	// next : UpdateBlock, ShaderResource change 하는것
}

void Texture::Render()
{
	if (!active)
		return;

	UINT stride = sizeof(Vertex);	// 정점 크기에대한 정보
	UINT offset = 0;

	// IA 단계 Input Assembly

	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->IASetInputLayout(shader->GetLayout());
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 나머지 단계는 Shader 에서 진행한다.

	shader->Draw(6, 0);
}

void Texture::UpdateBlock()
{
	// Vertices 정의
	Vertex vertices[6];

	// Vertices 설정
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

	// Map 과 UnMap 사이에서 데이터를 수정한다.
	D3D11_MAPPED_SUBRESOURCE subResource;
	DeviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	// Subresource 애 데이터를 넣을준비를한다.

	memcpy(subResource.pData, vertices, sizeof(Vertex) * 6);
	// Subresource 에 정점에 대한 데이터를 넣어놓는다.

	DeviceContext->Unmap(vertexBuffer, 0);
	// 다시 Unmap 을 해줘서 덮는다.
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
	// Vertices 정의
	Vertex vertices[6];

	// Vertices 설정
	{
		vertices[0].position = Vector3(-0.5f, -0.5f, 0.0f); // 0
		vertices[1].position = Vector3(-0.5f, +0.5f, 0.0f);	// 1
		vertices[2].position = Vector3(+0.5f, -0.5f, 0.0f);	// 3
		vertices[3].position = Vector3(+0.5f, -0.5f, 0.0f);	// 3
		vertices[4].position = Vector3(-0.5f, +0.5f, 0.0f);	// 1
		vertices[5].position = Vector3(+0.5f, +0.5f, 0.0f);	// 2

		// startX, endY 를 NCD 로 변경한다.
		{
			// 데이터 입력 받기

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

			// 비율계산

			startX = (startX > 0) ? startX / textureWidth : 0.0f;
			startY = (startY > 0) ? startY / textureHeight : 0.0f;

			endX = (endX > 0) ? endX / textureWidth : 1.0f;
			endY = (endY > 0) ? endY / textureHeight : 1.0f;

			if ((endX - startX) == 1.0f)
				spriteSize.x = textureWidth;

			if ((endY - startY) == 1.0f)
				spriteSize.y = textureHeight;
			// FULL

			// startX, startY -> NCD 좌표로 변환 완료

			vertices[0].uv = Vector2(startX, endY);
			vertices[1].uv = Vector2(startX, startY);
			vertices[2].uv = Vector2(endX, endY);
			vertices[3].uv = Vector2(endX, endY);
			vertices[4].uv = Vector2(startX, startY);
			vertices[5].uv = Vector2(endX, startY);
			// 기준 + : startX, endY  |  - : endX, startY
		}
	}


	// Buffer 로 연결
	{
		// desc 설정
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(Vertex) * 6;
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

void Texture::CreateIndexBuffer()
{
}

//////////////////////////////////////////////////////////
//
//	ImageFile 를 읽고 저장한다.
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

	// Shader Resource View 에는 3가지 Style 이 있다.
	// memory,
	// stream(dds), DX 에서 만들 파일 확장자이며 다른 함수들을 사용한다.
	// file(.png) 일반적인 이미지 파일을 사용한다.

	if (spriteResources.find(imageFile) != spriteResources.end())
		return;	// 이미 등록 되어있을 경우에 대한 예외 처리

	SpriteResource* spriteResource = new SpriteResource();

	// Image 정보 가져오기

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

	// Image size 입력

	// ImageFile 에 대한 info 를 가져온다.
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

	// srv 에 원래 있던 값 집어넣기
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
		// 이미 등록 되어있을 경우에 대한 예외 처리
	}

	return Vector2(1.0f, 1.0f);
}

ID3D11ShaderResourceView* Texture::FindShaderResourceView()
{
	if (spriteResources.find(imageFile) != spriteResources.end())
	{
		auto find = spriteResources.find(imageFile);
		return find->second-> srv;
		// 이미 등록 되어있을 경우에 대한 예외 처리
	}

	return nullptr;
}