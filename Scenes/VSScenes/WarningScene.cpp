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
		wstring str = L"광과민성요소 포함주의";

		float x = -(str.length() * 40.0f * 0.5f);
		float y = 20.0f;
		Vector2 pos = Vector2(x, y + 305.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 249, 66, 66, 40.0f);

		str = L"이 게임은 광점멸 요소가 있습니다. 현기증, 시력저하, 눈 혹은 안면 마비, 손발저림,";
		x = -(str.length() * 25.0f * 0.5f);
		pos = Vector2(x, y + 100.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 25.0f);

		str = L"방향감 상실, 착란, 순간적 의식상실을 경험하셨다면 바로 플레이를 중단하시고 의사와";
		x = -(str.length() * 25.0f * 0.5f);
		pos = Vector2(x + 25.0f, y + 50.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 25.0f);

		str = L"상담하십시오.";
		x = -460.0f;
		pos = Vector2(x, y);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 25.0f);

		str = L"발작 또는 간질의 경험 혹은 가족력이 있다면 플레이 전 의사와 상담하십시오.";
		x = -(str.length() * 25.0f * 0.5f);
		pos = Vector2(x - 21.25f, y - 75.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 25.0f);

		str = L"시작하려면 아무 키나 누르세요.";
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
