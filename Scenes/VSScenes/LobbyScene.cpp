#include "framework.h"
#include "LobbyScene.h"
#include "ObjectList.h"

#define   B_OPTION		1
#define   B_EXIT		2
#define   B_START		3
#define   B_POWERUP     4
#define   B_COLLECT     5
#define   B_CHALLANGE   6
#define   B_PROD        7
#define   B_BACK        8
#define   B_ANTONIO		9
#define	  B_CONFIRM		10

LobbyScene::LobbyScene()
{
	wstring imageFile1 = L"./Images/Background/IntroBG.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	// 메뉴바 (반투명)
	{
		menuBar = new Texture(imageFile1, shaderFile);
		menuBar->SetOffset(1.0f, 1.0f);
		menuBar->SetOffsetSize(1.0f, 1.0f);
		menuBar->SetScale(1532.0f, 133.333f);
		menuBar->SetPosition(0.0f, 456.666f);

		menuBar->UpdateColorBuffer(Color(0.0f, 0.0f, 0.0f, 0.5f), 4, 0.0f, 0.0f, 0.0f);
	}

	// 배경
	{
		imageFile1 = L"./Images/Background/IntroBG2.bmp";

		background = new Texture(imageFile1, shaderFile);
		background->SetScale(5.575f, 5.6225f);
		background->SetPosition(0.0f, 2.5f);
	}

	// 타이틀
	{
		wstring imageFile2 = L"./Images/Title.png";

		title = new Texture(imageFile2, shaderFile);
		title->SetPosition(0.0f, 192.5f);
		title->SetScale(1.355f, 1.355f);
	}

	// 일러스트
	{
		wstring imageFlie3 = L"./Images/illustrations.png";

		texture[0] = new Texture(imageFlie3, shaderFile);
		texture[0]->SetOffset(2.0f, 2.0f);
		texture[0]->SetOffsetSize(321.0f, 189.0f);
		texture[0]->SetPosition(0.0f, -212.5f);
		texture[0]->SetScale(2.7975f, 2.7975f);

		texture[1] = new Texture(imageFlie3, shaderFile);
		texture[1]->SetOffset(2.0f, 193.0f);
		texture[1]->SetOffsetSize(195.0f, 282.0f);
		texture[1]->SetPosition(-445.0f, -210.0f);
		texture[1]->SetScale(4.2295f, 4.2595f);
		texture[1]->UpdateColorBuffer(Color(0.0f, 0.0f, 0.0f, 0.5f), 7, 0.0f, 0.0f, 0.0f);

		texture[2] = new Texture(imageFlie3, shaderFile);
		texture[2]->SetOffset(199.0f, 193.0f);
		texture[2]->SetOffsetSize(212.0f, 200.0f);
		texture[2]->SetPosition(470.0f, 0.0f);
		texture[2]->SetRotation(0.0f, 180.0f, 0.0f);
		texture[2]->SetScale(2.755f, 2.855f);
		texture[2]->UpdateColorBuffer(Color(0.0f, 0.0f, 0.0f, 1.0f), 7, 0.0f, 0.0f, 0.0f);
	}

	wstring imageFile4 = L"./Images/UI.png";

	// Option 
	{
		imageFile4 = L"./Images/Option.png";
		window = new Texture(imageFile4, shaderFile);
		window->SetPosition(0.0f, -50.0f);
	}

	function<void(int)> callback;
	callback = bind(&LobbyScene::OnButtonClicked, this, placeholders::_1);

	for (int i = 0; i < 11; i++)
	{
		Button* bt = (Button*)OBJMANAGER->FindObject("Button" + to_string(i));

		switch (i)
		{
		case 0:
			bt->SetColor(0);
			bt->SetPosition(0.0f, 435.0f);
			bt->SetID(0);
			break;
		case 1:
			bt->SetColor(1);
			bt->SetButtonString(L"옵션");
			bt->SetPosition(282.5f, 435.0f);
			bt->SetID(1);
			break;
		case 2:
			bt->SetColor(2);
			bt->SetButtonString(L"나가기");
			bt->SetPosition(-282.5f, 435.0f);
			bt->SetID(2);
			break;
		case 3:
			bt->SetColor(3);
			bt->SetHover(true);
			bt->SetButtonString(L"시작");
			bt->SetPosition(0.0f, -130.0f);
			bt->SetID(3);
			break;
		case 4:
			bt->SetColor(4);
			bt->SetButtonString(L"파워 업");
			bt->SetPosition(0.0f, -300.0f);
			bt->SetID(4);
			break;
		case 5:
			bt->SetColor(5);
			bt->SetButtonString(L"컬렉션");
			bt->SetPosition(-255.0f, -300.0f);
			bt->SetID(5);
			break;
		case 6:
			bt->SetColor(5);
			bt->SetButtonString(L"도전 과제");
			bt->SetPosition(255.0f, -300.0f);
			bt->SetID(6);
			break;
		case 7:
			bt->SetColor(6);
			bt->SetButtonString(L"제작진");
			bt->SetButtonStringSize(20.0f);
			bt->SetPosition(2.5f, -435.0f);
			bt->SetID(7);
			break;
		case 8:
			bt->SetColor(2);
			bt->SetButtonString(L"뒤로");
			bt->SetPosition(282.5f, 435.0f);
			bt->SetID(8);
			break;
		case 9:
			bt->SetColor(8);
			bt->SetButtonString(L"안토니오");
			bt->SetButtonStringSize(27.5f);
			bt->SetPosition(-262.5f, 210.0f);
			bt->SetID(9);
			break;
		case 10:
			bt->SetColor(7);
			bt->SetButtonString(L"확인");
			bt->SetPosition(260.0f, -425.0f);
			bt->SetID(10);
			break;
		}
		bt->SetCallback(callback);
		button[i] = bt;
	}

	// SlideBox
	{
		slideBox[0] = (SlideBox*)OBJMANAGER->FindObject("SlideBox0");
		slideBox[1] = (SlideBox*)OBJMANAGER->FindObject("SlideBox1");

		//slideBox[0]->SetID(0);

		slideBox[0]->SetPosition(217.5f, 230.0f);
		slideBox[1]->SetPosition(217.5f, 125.0f);

		slideBox[0]->SetPercent(10.0f);
		slideBox[0]->SetPercent(10.0f);
	}

	// CheckBox
	{
		for (int i = 0; i < 5; i++)
			checkBox[i] = (CheckBox*)OBJMANAGER->FindObject("CheckBox" + to_string(i));
		
		float y = 17.5f;
		float gap = -107.5f;

		for (int i = 0; i < 5; i++)
		{
			checkBox[i]->SetPosition(277.5f, y + gap * i);
		}
	}

	// TagGroup Setting
	{
		button[B_START]->SetTopTabGroup(button[B_OPTION]);
		button[B_START]->SetBottomTabGroup(button[B_POWERUP]);

		button[B_OPTION]->SetBottomTabGroup(button[B_START]);
		button[B_OPTION]->SetLeftTabGroup(button[B_EXIT]);

		button[B_POWERUP]->SetTopTabGroup(button[B_START]);
		button[B_POWERUP]->SetBottomTabGroup(button[B_PROD]);
		button[B_POWERUP]->SetRightTabGroup(button[B_CHALLANGE]);
		button[B_POWERUP]->SetLeftTabGroup(button[B_COLLECT]);
		button[B_COLLECT]->SetLeftTabGroup(button[B_COLLECT]);

		button[B_CHALLANGE]->SetLeftTabGroup(button[B_POWERUP]);

		button[B_PROD]->SetLeftTabGroup(button[B_POWERUP]);
		button[B_COLLECT]->SetRightTabGroup(button[B_POWERUP]);
		button[B_PROD]->SetTopTabGroup(button[B_POWERUP]);

		button[B_EXIT]->SetRightTabGroup(button[B_OPTION]);

		list = (ListCtrl*)OBJMANAGER->FindObject("ListCtrl");
		list->SetTopTabGroup(button[B_BACK]);
	}

	// Collection Dialogue
	{
		wstring str = L"./Images/sDialogue.png";
		sDialogue = new Texture(str, shaderFile);
		sDialogue->SetPosition(0.0f, -400.0f);
		// -5 -400

		str = L"./Shader/HLSL/Color.hlsl";

		rect = new Rect(str);
		rect->SetPosition(0.0f, -400.0f);
		rect->SetScale(675.0f, 130.0f);
	}

	// EquipList
	{
		skillList = (EquipList*)OBJMANAGER->FindObject("EquipList");
	}

	SetActive(false);
	sceneName = "LobbyScene";
}

LobbyScene::~LobbyScene()
{
	for (int i = 0; i < 5; i++)
		if (checkBox[i])
			SAFE_DELETE(checkBox[i]);
	for (int i = 0; i < 10; i++)
		SAFE_DELETE(button[i]);
	for (int i = 0; i < 3; i++)
		SAFE_DELETE(texture[i]);
	SAFE_DELETE(title);
	SAFE_DELETE(background);
}

void LobbyScene::Update()
{
	CAMERA->Update();

	Matrix V = CAMERA->GetViewMatrix();
	Matrix P = CAMERA->GetProjMatrix();

	if (DOWN(VK_ESCAPE))
	{
		for (int i = 0; i < 11; i++)
			button[i]->SetHover(false);

		optionMenu = false;
		staffMenu = false;
		collectionMenu = false;
		selectCharMenu = false;

		button[B_START]->SetHover(true);
	}

	if (!enter)
	{
		if (DOWNANY)
			enter = true;

		if (boop)
		{
			btime += DELTA;
			if (btime >= 0.5f)
			{
				boop = false;
				btime = 0.0f;
			}
		}
		else
		{
			btime += DELTA;
			if (btime >= 0.5f)
			{
				boop = true;
				btime = 0.0f;
			}
		}

		background->Update(V, P);
		UpdateIL(V, P);
		title->Update(V, P);
	}
	else
	{
		UpdateBG(V, P);
		UpdateIL(V, P);
		UpdateBT(V, P);
	}

	if (optionMenu)
		OptionMenuUpdate();
	else if (staffMenu)
		StaffMenuUpdate();
	else if (collectionMenu)
		CollectionMenuUpdate();
	else if (selectCharMenu)
		SelectCharMenuUpdate();
	else
		GetBackToMainMenu();
}

void LobbyScene::Render()
{
	BEGINDRAW;

	if (!enter)
	{
		if (boop)
		{
			wstring str = L"시작하려면 아무 키나 누르세요.";
			float x = -(str.length() * 45.0f * 0.5f);

			Vector2 pos = Vector2(x, -325.0f);
			CAMERA->VCToWC(pos);

			DirectWrite::RenderText(str, pos, 255, 255, 255, 45.0f);
		}

		background->Render();
		RenderIL();
		title->Render();
	}
	else
	{
		RenderCoin();
		RenderBG();
		RenderIL();
		RenderBT();

		OptionMenuRender();
		StaffMenuRender();
		CollectionMenuRender();
		SelectCharMenuRender();

		if (!optionMenu && !staffMenu && !collectionMenu && !selectCharMenu)
		{
			wstring str = L"v0.5.0e - EA";
			float x = -(str.length() * 20.0f * 0.5f);

			Vector2 pos = Vector2(x + 267.5f, -412.5f);
			CAMERA->VCToWC(pos);

			DirectWrite::RenderText(str, pos, 255, 255, 255, 20.0f);
		}

	}

	ENDDRAW;
}

void LobbyScene::ChangeScene()
{
	PLAYSOUND("Intro", bgmSize);

	SetActive(true);

	slideBox[0]->SetTopTabGroup(button[B_BACK]);
	slideBox[0]->SetBottomTabGroup(slideBox[1]);

	slideBox[1]->SetTopTabGroup(slideBox[0]);
	slideBox[1]->SetBottomTabGroup(checkBox[0]);

	checkBox[0]->SetTopTabGroup(slideBox[1]);
	checkBox[0]->SetBottomTabGroup(checkBox[1]);

	checkBox[1]->SetTopTabGroup(checkBox[0]);
	checkBox[1]->SetBottomTabGroup(checkBox[2]);

	checkBox[2]->SetTopTabGroup(checkBox[1]);
	checkBox[2]->SetBottomTabGroup(checkBox[3]);

	checkBox[3]->SetTopTabGroup(checkBox[2]);
	checkBox[3]->SetBottomTabGroup(checkBox[4]);

	checkBox[4]->SetTopTabGroup(checkBox[3]);

	Player* pl = (Player*)OBJMANAGER->FindObject("Player");
	pl->SetActive(false);

	slideBox[0]->SetPosition(217.5f, 230.0f);
	slideBox[1]->SetPosition(217.5f, 125.0f);

	float y = 17.5f;
	float gap = -107.5f;

	for (int i = 0; i < 5; i++)
	{
		checkBox[i]->SetPosition(277.5f, y + gap * i);
	}

	CAMERA->SetObject(nullptr);
	CAMERA->SetPosition(0.0f, 0.0f);

	step = 0;
	time = 0.0f;
	timer = 1.5f;
	btime = 0.0f;
	enter = false;
	boop = true;
	click = false;
	optionMenu = false;
	staffMenu = false;
	collectionMenu = false;
	selectCharMenu = false;

	for (int i = 0; i < 9; i++)
	{
		if (button[i])
			button[i]->SetHover(false);
	}

	button[B_START]->SetHover(true);
}

void LobbyScene::UpdateBG(Matrix V, Matrix P)
{
	background->Update(V, P);
	title->Update(V, P);
	menuBar->Update(V, P);
}

void LobbyScene::UpdateIL(Matrix V, Matrix P)
{
	for (int i = 0; i < 3; i++)
		texture[i]->Update(V, P);
}

void LobbyScene::UpdateBT(Matrix V, Matrix P)
{
	for (int i = 0; i < 8; i++)
		button[i]->Update(V, P);
}

void LobbyScene::RenderBG()
{
	background->Render();
	title->Render();
	menuBar->Render();
}

void LobbyScene::RenderIL()
{
	texture[1]->Render();
	texture[2]->Render();
	texture[0]->Render();
}

void LobbyScene::RenderBT()
{
	for (int i = 0; i < 8; i++)
		button[i]->Render();
}

void LobbyScene::OptionMenuUpdate()
{
	Matrix V = CAMERA->GetViewMatrix();
	Matrix P = CAMERA->GetProjMatrix();

	for (int i = 1; i < 8; i++)
	{
		if (button[i]->IsActive())
			button[i]->SetActive(false);
	}

	for (int i = 0; i < 5; i++)
	{
		if (checkBox[i])
		{
			if (!checkBox[i]->IsActive())
				checkBox[i]->SetActive(true);
		}
	}

	for (int i = 0; i < 2; i++)
	{
		if (slideBox[i])
		{
			if (!slideBox[i]->IsActive())
				slideBox[i]->SetActive(true);
		}
	}

	button[B_BACK]->SetBottomTabGroup(slideBox[0]);

	button[B_BACK]->SetActive(true);
	button[B_BACK]->Update(V, P);
	window->Update(V, P);

	for (int i = 0; i < 5; i++)
		checkBox[i]->Update(V, P);

	slideBox[0]->Update(V, P);
	slideBox[1]->Update(V, P);
}

void LobbyScene::OptionMenuRender()
{
	if (optionMenu)
	{
		window->Render();

		button[B_BACK]->Render();

		checkBox[0]->Render();
		checkBox[1]->Render();
		checkBox[2]->Render();
		checkBox[3]->Render();
		checkBox[4]->Render();

		slideBox[0]->Render();
		slideBox[1]->Render();

		wstring str = L"옵션";
		float x = -(str.length() * 40.0f * 0.5f);
		float y = 20.0f;
		Vector2 pos = Vector2(x, y + 350.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 40.0f);

		str = L"사운드";
		x = -(str.length() * 35.0f * 0.5f);
		pos = Vector2(x - 275.0f, y + 230.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 35.0f);

		str = L"음악";
		x = -(str.length() * 35.0f * 0.5f);
		pos = Vector2(x - 290.0f, y + 127.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 35.0f);

		str = L"시각효과 (vfx)";
		x = -(str.length() * 35.0f * 0.5f);
		pos = Vector2(x - 215.0f, y + 17.5f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 35.0f);

		str = L"조이스틱 표시";
		x = -(str.length() * 35.0f * 0.5f);
		pos = Vector2(x - 215.0f, y - 90.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 35.0f);

		str = L"데미지 표시";
		x = -(str.length() * 35.0f * 0.5f);
		pos = Vector2(x - 232.5f, y - 200.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 35.0f);

		str = L"스트리밍 친화적 음악";
		x = -(str.length() * 35.0f * 0.5f);
		pos = Vector2(x - 157.5f, y - 300.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 35.0f);

		str = L"퍼포먼스 모드";
		x = -(str.length() * 35.0f * 0.5f);
		pos = Vector2(x - 215.0f, y - 400.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 35.0f);

		str = L"시각효과 (vfx)";
		x = -(str.length() * 35.0f * 0.5f);
		pos = Vector2(x - 215.0f, y + 17.5f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 35.0f);

	}
}

void LobbyScene::StaffMenuUpdate()
{
	Matrix V = CAMERA->GetViewMatrix();
	Matrix P = CAMERA->GetProjMatrix();

	for (int i = 1; i < 8; i++)
	{
		if (button[i]->IsActive())
			button[i]->SetActive(false);
	}

	button[B_BACK]->SetActive(true);
	button[B_BACK]->Update(V, P);
	window->Update(V, P);
}

void LobbyScene::StaffMenuRender()
{
	if (staffMenu)
	{
		window->Render();
		button[B_BACK]->Render();

		wstring str = L"제작진";
		float x = -(str.length() * 40.0f * 0.5f);

		Vector2 pos = Vector2(x, 350.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 40.0f);

		str = L"원작 : Vampire Survivors";
		x = -(str.length() * 35.0f * 0.5f);

		pos = Vector2(x - 144.0f, 275.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 35.0f);

		str = L"원제작자 : Luca Galante";
		x = -(str.length() * 35.0f * 0.5f);

		pos = Vector2(x - 150.0f, 200.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 35.0f);

		str = L"그래픽 : Images from Steam Local Asset";
		x = -(str.length() * 35.0f * 0.5f);

		pos = Vector2(x - 20.0f, 125.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 35.0f);

		str = L"사운드 : Sounds from Steam Local Asset";
		x = -(str.length() * 35.0f * 0.5f);

		pos = Vector2(x - 20.0f, 50.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 35.0f);


		str = L"프로그래밍 : 나";
		x = -(str.length() * 35.0f * 0.5f);

		pos = Vector2(x - 217.5f, -25.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 35.0f);
	}
}

void LobbyScene::CollectionMenuUpdate()
{
	Matrix V = CAMERA->GetViewMatrix();
	Matrix P = CAMERA->GetProjMatrix();

	window->Update(V, P);
	list->Update(V, P);
	for (int i = 1; i < 8; i++)
	{
		if (button[i]->IsActive())
			button[i]->SetActive(false);
	}
	button[B_BACK]->SetBottomTabGroup(list);
	button[B_BACK]->SetActive(true);
	button[B_BACK]->Update(V, P);
}

void LobbyScene::CollectionMenuRender()
{
	if (collectionMenu)
	{
		window->Render();
		list->Render();
		button[B_BACK]->Render();
	}
}

void LobbyScene::SelectCharMenuUpdate()
{
	Matrix V = CAMERA->GetViewMatrix();
	Matrix P = CAMERA->GetProjMatrix();

	if (button[B_BACK]->IsClick())
	{
		button[B_BACK]->SetClick(false);
	}
		
	for (int i = 1; i < 8; i++)
	{
		if (button[i]->IsActive())
			button[i]->SetActive(false);
	}

	window->Update(V, P);

	button[B_BACK]->SetActive(true);
	button[B_BACK]->SetBottomTabGroup(button[B_ANTONIO]);

	button[B_ANTONIO]->SetActive(true);
	button[B_ANTONIO]->SetTopTabGroup(button[B_BACK]);
	
	button[B_CONFIRM]->SetActive(true);

	if (charSelected)
	{
		if (!button[B_ANTONIO]->GetHover())
		{
			charSelected = false;
			return;
		}

		if (DOWN(VK_SPACE) || DOWN(VK_RETURN))
		{
			charSelected = false;
			GetBackToMainMenu();
			SCENEMANAGER->ChangeScene("InGameScene");

			return;
		}

		button[B_CONFIRM]->SetButtonString(L"시작");
		button[B_ANTONIO]->UpdateColorBuffer(Color(), 10, 0, 0, 0);
		sDialogue->UpdateColorBuffer(Color(), 10, 0, 0, 0);
	}
	else
	{
		button[B_CONFIRM]->SetButtonString(L"확인");
		button[B_ANTONIO]->UpdateColorBuffer(Color(), 0, 0, 0, 0);
		sDialogue->UpdateColorBuffer(Color(), 0, 0, 0, 0);
	}

	sDialogue->Update(V, P);
	button[B_BACK]->Update(V, P);
	button[B_ANTONIO]->Update(V, P);
	button[B_CONFIRM]->Update(V, P);
	skillList->SetRenderType(0);
	skillList->Update(V, P);
}

void LobbyScene::SelectCharMenuRender()
{
	if (selectCharMenu)
	{
		window->Render();
		sDialogue->Render();
		button[B_BACK]->Render();
		button[B_ANTONIO]->Render();
		button[B_CONFIRM]->Render();
		skillList->Render();

		wstring str = L"캐릭터 선택";
		float x = -(str.length() * 40.0f * 0.5f);

		Vector2 pos = Vector2(x, 375.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 40.0f);

		str = L"안토니오 벨파에제";
		x = -(str.length() * 22.5f * 0.5f);
		pos = Vector2(x - 235.0f, -335.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 22.5f);

		str = L"매 10레벨마다 피해량이";
		x = -(str.length() * 22.5f * 0.5f);
		pos = Vector2(x, -370.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 22.5f);

		str = L"10% 증가합니다.   (최대";
		x = -(str.length() * 22.5f * 0.5f);
		pos = Vector2(x, -395.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 22.5f);

		str = L"+ 50%)";
		x = -(str.length() * 22.5f * 0.5f);
		pos = Vector2(x - 82.5f, -420.0f);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 22.5f);

	}
}

void LobbyScene::GetBackToMainMenu()
{
	for (int i = 1; i < 8; i++)
	{
		if (i != 8)
			if (!button[i]->IsActive())
				button[i]->SetActive(true);
	}

	for (int i = 0; i < 5; i++)
	{
		if (checkBox[i])
		{
			if (checkBox[i]->IsActive())
				checkBox[i]->SetActive(false);
		}
	}

	for (int i = 0; i < 2; i++)
	{
		if (slideBox[i])
		{
			if (!slideBox[i]->IsActive())
				slideBox[i]->SetActive(false);
		}
	}

	button[B_BACK]->SetActive(false);
	button[B_ANTONIO]->SetActive(false);
	button[B_CONFIRM]->SetActive(false);

	return;
}

void LobbyScene::RenderCoin()
{
	Matrix V = CAMERA->GetViewMatrix();
	Matrix P = CAMERA->GetProjMatrix();
	Player* player = PLAYER;
	int val = player->GetGold();
	wstring str = to_wstring(player->GetGold());
	float x = -(str.length() * 35.0f * 0.5f);

	Vector2 pos = button[0]->GetPosition();

	if (val >= 10000)
		pos.x -= 5.0f;
	else if (val >= 1000)
		pos.x += 5.0f;
	else if (val >= 100)
		pos.x += 25.0f;
	else if (val >= 10)
		pos.x += 45.0f;
	else if (val == 0)
		pos.x += 60.0f;

	pos.y += 26.0f;

	CAMERA->VCToWC(pos);

	DirectWrite::RenderText(str, pos, 255, 255, 255, 35.0f);
}

void LobbyScene::OnButtonClicked(int buttonID)
{
	for (int i = 1; i < 9; i++)
		button[i]->Reset();

	switch (buttonID)
	{
	case B_OPTION:
		optionMenu = true;
		button[B_BACK]->SetHover(true);
		break;
	case B_EXIT:
		PostQuitMessage(0);
		break;
	case B_START:
		selectCharMenu = true;
		button[9]->SetHover(true);
		break;
	case B_COLLECT:
		collectionMenu = true;
		list->SetActive(true);
		list->SetHover(true);
		break;
	case B_PROD:
		staffMenu = true;
		button[8]->SetHover(true);
		break;
	case B_BACK:
		if (optionMenu)
		{
			optionMenu = false;
			button[B_OPTION]->SetHover(true);
		}
		else if (staffMenu)
		{
			staffMenu = false;
			button[B_PROD]->SetHover(true);
		}
		else if (collectionMenu)
		{
			collectionMenu = false;
			button[B_COLLECT]->SetHover(true);
		}
		else if (selectCharMenu)
		{
			selectCharMenu = false;
			button[B_START]->SetHover(true);
		}
		break;
	case B_ANTONIO:
		charSelected = true;
		break;
	}
}
