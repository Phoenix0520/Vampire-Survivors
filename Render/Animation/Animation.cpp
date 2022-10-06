#include "framework.h"
#include "Texture.h"
#include "Animation.h"

///////////////////////////////////////////////////////////////////////
//	������
///////////////////////////////////////////////////////////////////////
Animation::Animation(wstring imageFile, wstring shaderFile)
{
	texturePtr = new Texture(imageFile, shaderFile);
}

Animation::~Animation()
{
	SAFE_DELETE(texturePtr);
	for (UINT i = 0; i < aniClips.size(); i++)
	{
		SAFE_DELETE(aniClips[i]);
	}
	aniClips.erase(aniClips.begin(), aniClips.end());

}

void Animation::AddClip(AnimationClip* clip)
{
	clip->SetTexture(this->texturePtr);
	aniClips.push_back(clip);
}

void Animation::SetOffset(float x, float y)
{
	for (UINT i = 0; i < aniClips.size(); i++)
	{
		AnimationClip* clip = aniClips[i];
		clip->SetOffset(x, y);
	}
}


void Animation::SetOffsetSize(float x, float y)
{
	for (UINT i = 0; i < aniClips.size(); i++)
	{
		AnimationClip* clip = aniClips[i];
		clip->SetOffsetSize(x, y);
	}
}

///////////////////////////////////////////////////////////////////////////////
//	Player�� Update() ���� ������ Call �ϰԲ� ���ش�.
//
//
///////////////////////////////////////////////////////////////////////////////
void Animation::SetPlay(UINT play)
{
	// ������ Play �� �ѹ��� �ٲ����ʰ� ���־����
	AnimationClip* clip = nullptr;
	// �̻�ó��
	{
		if (aniClips.size() == 0)
			return;
		if (curClip >= aniClips.size())
			return;
	}

	clip = aniClips[curClip];

	if (clip->IsPlaying() && play == curClip)
		return;

	curClip = play;
	clip->SetPlay();
}

void Animation::SetStop()
{
	// �̻�ó��
	{
		if (aniClips.size() == 0)
			return;

		if (curClip >= aniClips.size())
			return;
	}
	AnimationClip* clip = aniClips[curClip];
	clip->SetStop();
}

//void Animation::SetUpdate(UINT state, Vector2 scale )
//{
//	this->SetPlay((UINT)state);
//	this->SetScale(scale);
//	this->SetPosition(position);
//	this->SetRotation(rotation);
//}

bool Animation::IsPlay()
{
	AnimationClip* clip = aniClips[curClip];
	return clip->IsPlaying();
}

void Animation::Update(Matrix V, Matrix P)
{
	// �̻�ó��
	{
		if (aniClips.size() == 0)
			return;

		if (curClip >= aniClips.size())
			return;
	}
	AnimationClip* clip = aniClips[curClip];
	clip->Update(V, P);
}

void Animation::Render()
{
	// �̻�ó��
	{
		if (aniClips.size() == 0)
			return;

		if (curClip >= aniClips.size())
			return;
	}
	AnimationClip* clip = aniClips[curClip];
	clip->Render();
}

void Animation::UpdateColorBuffer(Color color, int path, float time, float time2, float time3)
{
	texturePtr->UpdateColorBuffer(color, path, time, time2, time3);
}
