#include "framework.h"
#include "SceneManager.h"
#include "SceneList.h"
#include "ObjectList.h"
#include "SkillList.h"

////////////////////////////////////////////////////////
// 생성자, 소멸자
////////////////////////////////////////////////////////

SceneManager::SceneManager()
{
	scenes.push_back(new WarningScene());
	//scenes.push_back(new GameOverScene());
	
	//thread t1(bind(&SceneManager::ThreadStart1, this));
	//t1.detach();

	thread t2(bind(&SceneManager::ThreadStart2, this));
	t2.detach();
}

SceneManager::~SceneManager()
{
	for (UINT i = 0; i < scenes.size(); i++)
	{
		SAFE_DELETE(scenes.at(i));
	}

	scenes.erase(scenes.begin(), scenes.end());
}


void SceneManager::CreateObject()
{
	long first = GetTickCount();
	long start = GetTickCount();
	long end;

	// Sound Including
	{
		ADDSOUND("Projectile", "./Sounds/Projectile.wav", false);
		ADDSOUND("MagicWand", "./Sounds/MagicWand.wav", false);
		ADDSOUND("Hit", "./Sounds/Hit.wav", false);
		ADDSOUND("FoundChest", "./Sounds/FoundChest.wav", false);
		ADDSOUND("Chest1", "./Sounds/Chest1.wav", false);
		ADDSOUND("Chest2", "./Sounds/Chest2.wav", false);
		ADDSOUND("Chest3", "./Sounds/Chest3.wav", false);
		ADDSOUND("GameOver", "./Sounds/Gameover.wav", false);
		ADDSOUND("BGM", "./Sounds/MainBGM.wav", true);
		ADDSOUND("Intro", "./Sounds/Intro.wav", false);
		end = GetTickCount();

		cout << "Sounds : " << (float)(end - start) / 1000 << " sec" << endl;
	}

	// Player
	{
		Player* player = new Player();
		OBJMANAGER->AddObject("Player", player);
		end = GetTickCount();
		cout << "Player : " << (float)(end - start) / 1000 << " sec" << endl;
	}

	// SkillEffect
	{
		start = GetTickCount();
		for (int i = 0; i < 8; i++)
		{
			SkillEffect* se = nullptr;

			switch (i)
			{
			case 0:
				se = new Whip();
				break;
			case 1:
				se = new MagicWand();
				break;
			case 2:
				se = new Knife();
				break;
			case 3:
				se = new Axe();
				break;
			case 4:
				se = new Cross();
				break;
			case 5:
				se = new Bible();
				break;
			case 6:
				se = new FireWand();
				break;
			case 7:
				se = new Garlic();
				break;
			}
			
			if (se)
			{
				se->SetID(i);
				OBJMANAGER->AddObject("SkillEffect" + to_string(i), se);
			}
		}
		end = GetTickCount();
		cout << "SkillEffect : " << (float)(end - start) / 1000 << " sec" << endl;
	}

	// EquipList
	{
		start = GetTickCount();
		EquipList* sl = new EquipList();
		OBJMANAGER->AddObject("EquipList", sl);
		end = GetTickCount();
		cout << "EquipList : " << (float)(end - start) / 1000 << " sec" << endl;
		
	}

	// Cursor 
	{
		start = GetTickCount();
		Cursor* cursor = new Cursor();
		OBJMANAGER->AddObject("Cursor", cursor);
		Cursor* chestCursor = new Cursor();
		OBJMANAGER->AddObject("Cursor2", chestCursor);
		end = GetTickCount();
		cout << "Cursor : " << (float)(end - start) / 1000 << " sec" << endl;
		
	}

	// ListCtrl
	{
		start = GetTickCount();
		ListCtrl* lc = new ListCtrl();
		OBJMANAGER->AddObject("ListCtrl", lc);
		end = GetTickCount();
		cout << "ListCtrl : " << (float)(end - start) / 1000 << " sec" << endl;
		
	}

	// Button
	{
		start = GetTickCount();
		for (int i = 0; i < 14; i++)
		{
			Button* bt = new Button();
			OBJMANAGER->AddObject("Button" + to_string(i), bt);
		}
		end = GetTickCount();
		cout << "Button : " << (float)(end - start) / 1000 << " sec" << endl;
		
	}

	// SlideBox
	{
		start = GetTickCount();
		for (int i = 0; i < 2; i++)
		{
			SlideBox* sb = new SlideBox();
			OBJMANAGER->AddObject("SlideBox" + to_string(i), sb);
		}
		end = GetTickCount();
		cout << "SlideBox : " << (float)(end - start) / 1000 << " sec" << endl;
		
	}

	// CheckBox
	{
		start = GetTickCount();
		for (int i = 0; i < 5; i++)
		{
			CheckBox* cb = new CheckBox();
			OBJMANAGER->AddObject("CheckBox" + to_string(i), cb);
		}
		end = GetTickCount();
		cout << "CheckBox : " << (float)(end - start) / 1000 << " sec" << endl;
		
	}

	// HitEffect
	{
		start = GetTickCount();

		HitEffect* he = new HitEffect();
		OBJMANAGER->AddObject("HitEffect", he);

		end = GetTickCount();
		cout << "HitEffect : " << (float)(end - start) / 1000 << " sec" << endl;
		
	}

	// Monster
	{
		start = GetTickCount();
		for (int i = 0; i < MAX_MOB; i++)
		{
			Monster* mb = new Monster();
			OBJMANAGER->AddObject("Monster" + to_string(i), mb);
		}
		end = GetTickCount();
		cout << "Monster : " << (float)(end - start) / 1000 << " sec" << endl;
		
	}

	// LvUpEffect
	{
		//start = GetTickCount();
		//
		//LvUpEffect* le = new LvUpEffect();
		//OBJMANAGER->AddObject("LvUpEffect", le);
		//
		//end = GetTickCount();
		//cout << "LvUpEffect : " << (float)(end - start) / 1000 << " sec" << endl;
		//
	}


	// DropItem
	{
		start = GetTickCount();
		for (int i = 0; i < MAX_ITEM; i++)
		{
			DropItem* item = new DropItem();

			item->SetID(i);
			OBJMANAGER->AddObject("DropItem" + to_string(i), item);
		}
		end = GetTickCount();
		cout << "DropItem : " << (float)(end - start) / 1000 << " sec" << endl;
	}

	// InGameUI
	{
		start = GetTickCount();
		
		InGameUI* ui = new InGameUI();
		OBJMANAGER->AddObject("InGameUI", ui);
		
		end = GetTickCount();
		cout << "InGameUI : " << (float)(end - start) / 1000 << " sec" << endl;

	}

	// Brazier
	{
		start = GetTickCount();

		for (int i = 0; i < MAX_ITEM; i++)
		{
			Brazier* br = new Brazier();
			OBJMANAGER->AddObject("Brazier" + to_string(i), br);
		}

		end = GetTickCount();
		cout << "Brazier : " << (float)(end - start) / 1000 << " sec" << endl;

	}

	cout << "Total Time : " << (float)(end - first) / 1000 << " sec" << endl;

	complete = true;
}

void SceneManager::Update()
{
	for (UINT i = 0; i < scenes.size(); i++)
	{
		if (scenes[i]->IsActive())
		{
			scenes[i]->Update();

			curScene = scenes[i];
			curSceneNum = i;
		}
	}

	// 소리크기 조정
	{
		SlideBox* sb1 = (SlideBox*)OBJMANAGER->FindObject("SlideBox0");
		SlideBox* sb2 = (SlideBox*)OBJMANAGER->FindObject("SlideBox1");

		if (!sb1 || !sb2)
			return;

		float val1 = sb1->GetPercent() / 500.0f;
		sfxSize = val1;
		audio->Volume("GameOver", sfxSize);
		audio->Volume("Fire", sfxSize);
		audio->Volume("FindChest", sfxSize);

		float val2 = sb2->GetPercent() / 500.0f;
		bgmSize = val2;
		audio->Volume("BGM", bgmSize);
		audio->Volume("Chest1", bgmSize);
		audio->Volume("Chest2", bgmSize);
		audio->Volume("Chest3", bgmSize);

		CheckBox* cb = (CheckBox*)OBJMANAGER->FindObject("CheckBox2");
		if (!cb)
			return;

		cout << cb->IsCheck() << endl;

		if (cb->IsCheck())
			visibleDamageCount = true;
		else
			visibleDamageCount = false;
	}
}

void SceneManager::Render()
{
	for (UINT i = 0; i < scenes.size(); i++)
	{
		if (scenes[i]->IsActive())
		{
			scenes[i]->Render();

			if (visibleStatus)
				RenderChapterDisplay();

			if (visibleObjID)
			{
				for (int i = 0; i < MAX_MOB; i++)
				{
					Monster* mob = (Monster*)OBJMANAGER->FindObject("Monster" + to_string(i));
					if (!mob->IsActive())
						continue;

					BEGINDRAW;

					Vector2 pos = mob->GetPosition();

					float damage	= mob->GetDamage();
					float hp		= mob->GetHp();
					float moveSpeed = mob->GetMoveSpeed();
					
					pos += Vector2(25.0f, -20.0f);
					CAMERA->VCToWC(pos);
					DirectWrite::RenderText(to_wstring(i), pos, 255, 0, 255, 15.0f);

					pos.x += 15.0f;
					DirectWrite::RenderText(to_wstring(moveSpeed, L"%.3f"), pos, 0, 255, 255, 15.0f);

					pos.y -= 25.0f;
					DirectWrite::RenderText(to_wstring(damage, L"%.3f"), pos, 255, 255, 0, 15.0f);

					pos.y -= 25.0f;
					DirectWrite::RenderText(to_wstring(hp, L"%.3f"), pos, 255, 0, 0, 15.0f);

					ENDDRAW;
				}

				for (int i = 0; i < MAX_ITEM; i++)
				{
					DropItem* item = (DropItem*)OBJMANAGER->FindObject("DropItem" + to_string(i));
					
					if (!item->IsActive())
						continue;

					BEGINDRAW;
					
					Vector2 pos = item->GetPosition();
					float val = 15.0f;
					pos += Vector2(val, -val);
					CAMERA->VCToWC(pos);

					DirectWrite::RenderText(to_wstring(i), pos, 255, 0, 255, val);
					
					ENDDRAW;
				}
			}
		}
	}


}

void SceneManager::ThreadStart1()
{
}

void SceneManager::ThreadStart2()
{
	CreateObject();

	scenes.push_back(new LobbyScene());
	scenes.push_back(new GameOverScene());
	scenes.push_back(new InGameScene());
}

void SceneManager::RenderChapterDisplay()
{
	BEGINDRAW;
	{
		RECT rect = { 5, 5, 505, 505 };
		wstring str = L"FPS : " + to_wstring(TIMEMANAGER->GetFrame());
		DirectWrite::RenderText(str, rect);

		rect.top += 20;
		rect.bottom += 20;

		str = L"CameraPos(" + to_wstring(CAMERA->GetPosition().x) + L" , " + to_wstring(CAMERA->GetPosition().y) + L")";
		DirectWrite::RenderText(str, rect);

		rect.top += 20;
		rect.bottom += 20;

		Vector2 pos = mouse->GetPosition();
		CAMERA->WCToVC(pos);

		str = L"MousePos(" + to_wstring(pos.x - CAMPOS.x) + L" , " + to_wstring(pos.y - CAMPOS.y) + L")";
		DirectWrite::RenderText(str, rect);

		rect.top += 20;
		rect.bottom += 20;

		str = L"SoundSize(" + to_wstring(bgmSize) + L" , " + to_wstring(sfxSize) + L")";
		DirectWrite::RenderText(str, rect);

		rect.top += 20;
		rect.bottom += 20;

		string name = GetCurScene()->GetSceneName();
		wstring str2;
		str2.assign(name.begin(), name.end());
		str = L"CurScene : " + str2;
		DirectWrite::RenderText(str, rect);

		Player* pl = (Player*)OBJMANAGER->FindObject("Player");
		if (!pl)
		{
			ENDDRAW;
			return;
		}

		if (pl->IsActive())
		{
			int hp = (int)pl->GetHp();
			int maxHp = (int)pl->GetMaxHp();

			str = L"HP : " + to_wstring(hp) + L" / " + to_wstring(maxHp);
			Vector2 pos = pl->GetPosition();
			pos.x -= str.length() * 10.0f;
			pos.y -= 50.0f;
			CAMERA->VCToWC(pos);

			//DirectWrite::RenderText(str, pos, 255, 255, 255, 20.0f);
		}
	}
	ENDDRAW;
}

Scene* SceneManager::GetScene(string name)
{
	Scene* value = nullptr;

	for (UINT i = 0; i < scenes.size(); i++)
	{
		if (scenes[i]->GetSceneName() != name)
			continue;

		if (scenes[i])
			value = scenes[i];
		else
			value = nullptr;

		break;
	}

	return value;
}

void SceneManager::ChangeScene(string name)
{
	for (UINT i = 0; i < scenes.size(); i++)
	{
		scenes[i]->SetActive(false);

		if (scenes[i] == curScene)
		{
			latScene = scenes[i];
			latSceneNum = i;
		}

		if (scenes[i]->GetSceneName() == name)
		{
			scenes[i]->SetActive(true);
			scenes[i]->ChangeScene();
		}
	}
}