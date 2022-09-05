#pragma once

class SRVManager
{
public:
	static  SRVManager* GetInstance()
	{
		static SRVManager instance;
		return &instance;
	}

public:
	// ����
	ID3D11ShaderResourceView*  FindShaderResourceView(wstring strFileName);
	Vector2                    FindSpriteSize(wstring strFileName);
	ID3D11ShaderResourceView*  CreateShaderResourceView(wstring strImageFile);

private:
	struct SpriteResource
	{
		ID3D11ShaderResourceView *m_pSRV;
		Vector2 m_SpriteSize;
	};
	map<wstring, SpriteResource*>  m_cmSpriteResources;  // Image File�� ���� ������ ����

private:
	SRVManager();
public:
	~SRVManager();

};
