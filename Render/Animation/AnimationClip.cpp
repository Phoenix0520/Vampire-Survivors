#include "framework.h"
#include "AnimationClip.h"

AnimationClip::AnimationClip(State state)
{
	this->state = state;
}

AnimationClip::~AnimationClip()
{
	for (UINT i = 0; i < frames.size(); i++)
	{
		SAFE_DELETE(frames[i]);
	}
	frames.erase(frames.begin(), frames.end());
}

void AnimationClip::Update(Matrix V, Matrix P)
{
	// ����ó��
	{
		if (frames.size() == 0)
			return;
		// �����ӿ� ���� �������

		if (frames.size() <= curFrame)
			return;
		// ������ �������ϰ��
	}

	Frame* frame = frames[curFrame];	// image, delta

	// �ð� üŷ
	if (IsPlaying() == true)
	{
		playTime = playTime + TIMEMANAGER->Delta();
		if (playTime >= frame->time)
		{
			curFrame++;	// ���� �̹��� index
			if (curFrame == frames.size())
			{
				if (state == State::once)
				{
					SetStop();
					curFrame = 0;
				}
				else if (state == State::end)
				{
					SetStop();
					curFrame = frames.size() - 1;
				}
				else if (state == State::endStay)
				{
					curFrame = frames.size() - 1;
				}
				else
					curFrame = 0;
			}
			playTime = 0.0f;
		}
	}

	// Frame
	frame = frames[curFrame];	// offset, offsetSize
	
	texturePtr->SetImageFile(frame->imageFile);
	texturePtr->SetOffset(frame->offset);
	texturePtr->SetOffsetSize(frame->offsetSize);
	// Texture �� �����͸� �����Ų��.
	texturePtr->Update(V, P);
}

void AnimationClip::Render()
{
	// ����ó��
	{
		if (frames.size() == 0)
			return;
		// �����ӿ� ���� �������

		if (frames.size() <= curFrame)
			return;
		// ������ �������ϰ��
	}

	texturePtr->Render();
}

void AnimationClip::UpdateColorBuffer(Color color, int path, float time, float time2, float time3)
{
	texturePtr->UpdateColorBuffer(color, path, time, time2, time3);
}

void AnimationClip::AddFrame(class Texture* texture, wstring imageFile, float startX, float startY, float endX, float endY, float delta)
{
	Frame* frame = new Frame();
	
	frame->imageFile	= imageFile;
	frame->offset		= Vector2(startX, startY);
	frame->offsetSize	= Vector2(endX - startX, endY - startY);
	frame->time			= delta;

	texture->CreateShaderResourceView(imageFile);

	frames.push_back(frame);
	texturePtr = texture;
}

void AnimationClip::AddFrame(wstring imageFile, float startX, float startY, float endX, float endY, float delta)
{
	Frame* frame = new Frame();

	frame->imageFile = imageFile;
	frame->offset = Vector2(startX, startY);
	frame->offsetSize = Vector2(endX - startX, endY - startY);
	frame->time = delta;

	if (texturePtr == nullptr)
	{
		MessageBox(NULL, L"Texture�� �������� �ʽ��ϴ�", nullptr, 0);
	}
		

	texturePtr->CreateShaderResourceView(imageFile);
	frames.push_back(frame);
}

void AnimationClip::SetOffset(float x, float y)
{
	for (UINT i = 0; i < frames.size(); i++)
	{
		Frame* frame = frames[i];
		frame->offset = Vector2(x, y);
	}
}

void AnimationClip::SetOffsetSize(float x, float y)
{
	for (UINT i = 0; i < frames.size(); i++)
	{
		Frame* frame = frames[i];
		frame->offsetSize = Vector2(x, y);
	}
}