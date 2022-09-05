#pragma once

///////////////////////////////////////////////////////////////////
//
//	애니메이션 종류
//	Animation
//	ㄴ AnimationClip
//	   ㄴ Texture
//
///////////////////////////////////////////////////////////////////

class AnimationClip	
{
public: // 열거형 
	enum State
	{
		loop = 0,	// IDLE		행동을 반복한다.
		once,		// Attack	행동을 한번만 시행한다.	첫번째 그림
		end,		// Attack	행동이 끝난후의 상태		마지막 그림
		endStay
	};

public: // 생성 소멸자
	AnimationClip(State state = State::loop);	// 기본값은 loop 이다
	~AnimationClip();

public: // 전역 함수
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

private: // 구조체
	struct Frame
	{
		wstring imageFile;
		Vector2 offset		= Vector2(0.0f, 0.0f);	// Texture::UpdateBlock()
		Vector2 offsetSize  = Vector2(0.0f, 0.0f);	// Texture::UpdateBlock()
		float	time		= 0.0f;					// 동작 시간
	};


private: // 멤버 변수
	class Texture*	texturePtr	= nullptr;			// 여기서 생성하지않음
	vector<Frame*>	frames;							// 프레임들을 담을 배열
	State			state		= State::loop;		// 초기 상태
	UINT			curFrame;						// 현재 프레임
	float			playTime	= 0.0f;				// 플레이 시간
	bool			play		= true;				// 애니메이션이 진행중인지 동작이 끝나면 false 가 된다.
};