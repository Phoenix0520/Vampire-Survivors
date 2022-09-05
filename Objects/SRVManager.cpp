#include "framework.h"
#include "SRVManager.h"
#include <io.h>

//////////////////////////////////////////////////////////
// 생성자
//////////////////////////////////////////////////////////
SRVManager::SRVManager()
{
}
//////////////////////////////////////////////////////////
// 소멸자
//////////////////////////////////////////////////////////
SRVManager::~SRVManager()
{
	for (auto v : m_cmSpriteResources)
	{
		SAFE_RELEASE(v.second->m_pSRV);
		delete v.second;
	}
	m_cmSpriteResources.clear();
}
ID3D11ShaderResourceView * SRVManager::CreateShaderResourceView(wstring strImageFile)
{
	string str;
	str.assign(strImageFile.begin(), strImageFile.end());

	if (_access(str.c_str(), 00) != 0)
	{
		str = " SRVManager::CreateShaderResourceView  File not found " + str;
		MessageBoxA(NULL, str.c_str(), "Game", MB_OK);
		return nullptr;
	}

	// Shader Resource View 3가지 Style : memory(dds), stream(dds), file(*.png)

	if (m_cmSpriteResources.find(strImageFile) != m_cmSpriteResources.end())
	{
		auto   find = m_cmSpriteResources.find(strImageFile);
		return find->second->m_pSRV;
	}


	SpriteResource* pSpriteResource = new SpriteResource();

	auto hr = D3DX11CreateShaderResourceViewFromFile(Device, strImageFile.c_str(),
		NULL, NULL, &pSpriteResource->m_pSRV, NULL);
	assert(SUCCEEDED(hr));

	// size

	D3DX11_IMAGE_INFO ImageInfo;
	hr = D3DX11GetImageInfoFromFile(strImageFile.c_str(), NULL, &ImageInfo, NULL);
	assert(SUCCEEDED(hr));

	pSpriteResource->m_SpriteSize = Vector2((float)ImageInfo.Width, (float)ImageInfo.Height);
	m_cmSpriteResources.insert(make_pair(strImageFile, pSpriteResource));
	return pSpriteResource->m_pSRV;

}

ID3D11ShaderResourceView * SRVManager::FindShaderResourceView(wstring strFileName)
{
	if (m_cmSpriteResources.find(strFileName) != m_cmSpriteResources.end())
	{
		auto find = m_cmSpriteResources.find(strFileName);
		return find->second->m_pSRV;
	}
	return nullptr;
}

Vector2 SRVManager::FindSpriteSize(wstring strFileName)
{
	if (m_cmSpriteResources.find(strFileName) != m_cmSpriteResources.end())
	{
		auto find = m_cmSpriteResources.find(strFileName);
		return find->second->m_SpriteSize;
	}
	return Vector2(0.0f, 0.0f);
}