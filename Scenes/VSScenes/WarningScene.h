#pragma once

class WarningScene : public Scene
{
public:
	WarningScene();
	~WarningScene();

public:
	void Update() override;
	void Render() override;
	void ChangeScene() override;

private:
	class Animation* animation = nullptr;

	float time = 0.0f;
	float mtime[50];
};