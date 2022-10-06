#include "framework.h"
#include "Texture.h"
#include "Animation.h"

///////////////////////////////////////////////////////////////////////
//	생성자
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
//	Player의 Update() 에서 무조건 Call 하게끔 해준다.
//
//
///////////////////////////////////////////////////////////////////////////////
void Animation::SetPlay(UINT play)
{
	// 기존에 Play 된 넘버와 바뀌지않게 해주어야함
	AnimationClip* clip = nullptr;
	// 이상처리
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
	// 이상처리
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
	// 이상처리
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
	// 이상처리
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
