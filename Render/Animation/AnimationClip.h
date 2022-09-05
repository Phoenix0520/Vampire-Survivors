#pragma once

///////////////////////////////////////////////////////////////////
//
//	�ִϸ��̼� ����
//	Animation
//	�� AnimationClip
//	   �� Texture
//
///////////////////////////////////////////////////////////////////

class AnimationClip	
{
public: // ������ 
	enum State
	{
		loop = 0,	// IDLE		�ൿ�� �ݺ��Ѵ�.
		once,		// Attack	�ൿ�� �ѹ��� �����Ѵ�.	ù��° �׸�
		end,		// Attack	�ൿ�� �������� ����		������ �׸�
		endStay
	};

public: // ���� �Ҹ���
	AnimationClip(State state = State::loop);	// �⺻���� loop �̴�
	~AnimationClip();

public: // ���� �Լ�
	void Update(Matrix V, Matrix P);
	void Render();
	void UpdateColorBuffer(Color color, int path, float time, float time2, float time3);
	//Update Render

	bool IsPlaying() { return play; }
	class Texture* GetTexture() { return texturePtr; }
	// Getter

	void AddFrame(class Texture* texture, wstring imageFile, float startX, float startY, float endX, float endY, float delta);
	void AddFrame(wstring imageFile, float startX, float startY, float endX, float endY, float delta);
	void SetPosition(float x, float y) { texturePtr->SetPosition(x, y); }
	void SetPosition(Vector2 pos) { texturePtr->SetPosition(pos); }
	void SetScale(float x, float y) { texturePtr->SetScale(x, y); }
	void SetScale(Vector2 scale) { texturePtr->SetScale(scale); }
	void SetRotation(float x, float y, float z) { texturePtr->SetRotation(x, y, z); }
	void SetOffset(float x, float y);
	void SetOffsetSize(float x, float y);
	void SetStop() { play = false; }
	void SetTexture(class Texture* texture) { texturePtr = texture; }
	void SetPlay() { curFrame = 0, playTime = 0.0f, play = true; }
	// Setter

private: // ����ü
	struct Frame
	{
		wstring imageFile;
		Vector2 offset		= Vector2(0.0f, 0.0f);	// Texture::UpdateBlock()
		Vector2 offsetSize  = Vector2(0.0f, 0.0f);	// Texture::UpdateBlock()
		float	time		= 0.0f;					// ���� �ð�
	};


private: // ��� ����
	class Texture*	texturePtr	= nullptr;			// ���⼭ ������������
	vector<Frame*>	frames;							// �����ӵ��� ���� �迭
	State			state		= State::loop;		// �ʱ� ����
	UINT			curFrame;						// ���� ������
	float			playTime	= 0.0f;				// �÷��� �ð�
	bool			play		= true;				// �ִϸ��̼��� ���������� ������ ������ false �� �ȴ�.
};