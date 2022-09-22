#include "framework.h"
#include "WarningScene.h"
#include "ObjectList.h"

WarningScene::WarningScene()
{
	//SetActive(false);
	sceneName = "WarningScene";

	wstring imageFile1 = L"./Images/Player.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	animation = new Animation(imageFile1, shaderFile);

	// Set Clip
	{
		AnimationClip* clip = new AnimationClip(AnimationClip::State::loop);
		animation->AddClip(clip);

		clip->AddFrame(animation->GetTexture(), imageFile1, 0.0f, 0.0f, 28.0f, 32.0f, 0.125f);
		clip->AddFrame(animation->GetTexture(), imageFile1, 29.0f, 0.0f, 58.0f, 32.0f, 0.125f);
		clip->AddFrame(animation->GetTexture(), imageFile1, 59.0f, 0.0f, 89.0f, 32.0f, 0.125f);
		clip->AddFrame(animation->GetTexture(), imageFile1, 91.0f, 0.0f, 119.0f, 32.0f, 0.125f);
	}

	animation->SetScale(1.55f, 1.575f);
	animation->SetPosition(715.0f, -415.0f);
}

WarningScene::~WarningScene()
{
	SAFE_DELETE(animation);
}

void WarningScene::Update()
{
	CAMERA->Update();

	Matrix V = CAMERA->GetViewMatrix();
	Matrix P = CAMERA->GetProjMatrix();

	if (DOWN(VK_SPACE))
	{
		if (SCENEMANAGER->GetScene("LobbyScene"))
			SCENEMANAGER->ChangeScene("LobbyScene");
	}
	
	time += DELTA;

	static float tTime = 0.0f;
	static float pTime = 0.0f;

	mt19937	engine((unsigned int)std::time(NULL));
	uniform_int_distribution<> distribution(0, 150);
	auto generator = bind(distribution, engine);

	tTime -= DELTA;
	pTime -= DELTA;

	if (tTime > pTime)
	{
		animation->SetStop();
	}
	else
	{
		animation->SetPlay(0);
	}
		
	if (tTime <= 0.0f && pTime <= 0.0f)
	{
		tTime = (float)generator() / 100.0f;
		pTime = (float)generator() / 100.0f;
	}

	animation->Update(V, P);
}

void WarningScene::Render()
{
	if (!SCENEMANAGER->IsCompleteCreate())
		animation->Render();
	else
	{


		BEGINDRAW;
		wstring str = L"�����μ���� ��������";

		float x = -(str.length() * 40.0f * 0.5f);
		float y = 20.0f;
		Vector2 pos = Vector2(x, y + 305.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 249, 66, 66, 40.0f);

		str = L"�� ������ ������ ��Ұ� �ֽ��ϴ�. ������, �÷�����, �� Ȥ�� �ȸ� ����, �չ�����,";
		x = -(str.length() * 25.0f * 0.5f);
		pos = Vector2(x, y + 100.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 25.0f);

		str = L"���Ⱘ ���, ����, ������ �ǽĻ���� �����ϼ̴ٸ� �ٷ� �÷��̸� �ߴ��Ͻð� �ǻ��";
		x = -(str.length() * 25.0f * 0.5f);
		pos = Vector2(x + 25.0f, y + 50.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 25.0f);

		str = L"����Ͻʽÿ�.";
		x = -460.0f;
		pos = Vector2(x, y);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 25.0f);

		str = L"���� �Ǵ� ������ ���� Ȥ�� �������� �ִٸ� �÷��� �� �ǻ�� ����Ͻʽÿ�.";
		x = -(str.length() * 25.0f * 0.5f);
		pos = Vector2(x - 21.25f, y - 75.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 25.0f);

		str = L"�����Ϸ��� �ƹ� Ű�� ��������.";
		x = -(str.length() * 30.0f * 0.5f);
		pos = Vector2(x, y - 305.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 30.0f);

		ENDDRAW;
	}
}

void WarningScene::ChangeScene()
{
}
