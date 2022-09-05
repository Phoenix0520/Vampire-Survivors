#pragma once

class Animation
{
public:
	Animation(wstring imageFile, wstring shaderFile);
	~Animation();

public:
	void Update(Matrix V, Matrix P);
	void Render();
	void UpdateColorBuffer(Color color, int path, float time, float time2, float time3);
	// Update Render

	void AddClip(class AnimationClip* clip);
	void SetPosition(float x, float y) { texturePtr->SetPosition(x, y); }
	void SetPosition(Vector2 pos) { texturePtr->SetPosition(pos); }
	void SetScale(float x, float y) { texturePtr->SetScale(x, y); }
	void SetScale(Vector2 scale) { texturePtr->SetScale(scale); }
	void SetRotation(float x, float y, float z) { texturePtr->SetRotation(x, y, z); }
	void SetRotation(Vector3 rot) { texturePtr->SetRotation(rot); }
	void SetOffset(float x, float y);
	void SetOffsetSize(float x, float y);
	void SetPlay(UINT play);	// AnimationClip 의 index 변경
	void SetStop();				// 강제적으로 Stop 할경우
	//void SetUpdate();
	// Setter

	bool IsPlay();
	class Texture* GetTexture() { return texturePtr; }
	Vector2 GetPosition() { return texturePtr->GetPosition(); }
	Vector2 GetScale() { return texturePtr->GetScale(); }
	Vector3 GetRotation() { return texturePtr->GetRotation(); }
	Vector2 GetTextureRealSize() { return texturePtr->GetTextureRealSize(); }
	// Getter
	
private:
	class Texture*					texturePtr	= nullptr;
	vector<class AnimationClip*>	aniClips;
	UINT							curClip	= 0;
};