#include "framework.h"
#include "Monster.h"
#include "ObjectList.h"
#include "HitEffect.h"
#include "ScreenEffect.h"

Monster::Monster()
{
	wstring imageFile1 = L"./Images/Monsters/Bat/BatIDLE.png";
	wstring imageFile2 = L"./Images/Monsters/Bat/BatDead.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	animation = new Animation(imageFile1, shaderFile);
	SetScale(2.5f, 2.5f);
	animation->SetScale(scale);

	// Move ( IDLE )
	{
		AnimationClip* clip = new AnimationClip(AnimationClip::loop);
		animation->AddClip(clip);

		clip->AddFrame(animation->GetTexture(), imageFile1, 0.0f, 0.0f, 23.0f, 22.0f, 0.125f);
		clip->AddFrame(animation->GetTexture(), imageFile1, 24.0f, 0.0f, 44.0f, 22.0f, 0.125f);
		clip->AddFrame(animation->GetTexture(), imageFile1, 44.0f, 0.0f, 61.0f, 22.0f, 0.125f);
		clip->AddFrame(animation->GetTexture(), imageFile1, 62.0f, 0.0f, 81.0f, 22.0f, 0.125f);
	}
	// Dead
	{
		AnimationClip* clip = new AnimationClip(AnimationClip::end);
		animation->AddClip(clip);
		
		clip->AddFrame(animation->GetTexture(), imageFile2, 0.0f, 0.0f, 22.0f, 21.0f, 0.04f);
		clip->AddFrame(animation->GetTexture(), imageFile2, 23.0f, 0.0f, 46.0f, 21.0f, 0.04f);
		clip->AddFrame(animation->GetTexture(), imageFile2, 47.0f, 0.0f, 70.0f, 21.0f, 0.04f);
		clip->AddFrame(animation->GetTexture(), imageFile2, 71.0f, 0.0f, 98.0f, 21.0f, 0.04f);
		clip->AddFrame(animation->GetTexture(), imageFile2, 99.0f, 0.0f, 124.0f, 23.0f, 0.04f);
		clip->AddFrame(animation->GetTexture(), imageFile2, 124.0f, 0.0f, 153.0f, 27.0f, 0.04f);
		clip->AddFrame(animation->GetTexture(), imageFile2, 154.0f, 0.0f, 182.0f, 30.0f, 0.04f);
		clip->AddFrame(animation->GetTexture(), imageFile2, 183.0f, 0.0f, 209.0f, 34.0f, 0.04f);
		clip->AddFrame(animation->GetTexture(), imageFile2, 210.0f, 0.0f, 238.0f, 31.0f, 0.04f);
		clip->AddFrame(animation->GetTexture(), imageFile2, 239.0f, 0.0f, 270.0f, 28.0f, 0.04f);
		clip->AddFrame(animation->GetTexture(), imageFile2, 271.0f, 0.0f, 296.0f, 30.0f, 0.04f);
		clip->AddFrame(animation->GetTexture(), imageFile2, 297.0f, 0.0f, 321.0f, 30.0f, 0.04f);
		clip->AddFrame(animation->GetTexture(), imageFile2, 322.0f, 0.0f, 352.0f, 27.0f, 0.04f);
		clip->AddFrame(animation->GetTexture(), imageFile2, 353.0f, 0.0f, 386.0f, 9.0f, 0.04f);
	}

	time = 1.0f;

	hitEffect = (HitEffect*)OBJMANAGER->FindObject("HitEffect");

	collider = new Collider();
	SetActive(false);
}

Monster::~Monster()
{
	SAFE_DELETE(animation);
}

void Monster::Update(Matrix V, Matrix P)
{
	if (!IsActive())
		return;
	
	if (type == NONE)
	{
		SetActive(false);
		cout << "\'None\' 은 생성되지 않습니다." << endl;
		return;
	}

	if (dead)
	{
		cout << "몬스터가 사망했습니다." << endl;
		SetActive(false);
		Player* pl = (Player*)OBJMANAGER->FindObject("Player");
		pl->SetKill(pl->GetKill() + 1);
		
		for (int i = 0; i < MAX_ITEM; i++)
		{
			DropItem* item = (DropItem*)OBJMANAGER->FindObject("DropItem" + to_string(i));

			if (!item->IsActive())
			{
				cout << "몬스터가 경험치 보석을 드롭했습니다." << endl;
				item->Reset();
				item->SetType(DropItem::EXP);
				item->SetPosition(this->GetPosition());
				return;
			}
		}
		cout << "아이템 최대 수용 가능량이 가득차 더이상 소환되지 않습니다." << endl;
		return;
	}

	switch (state)
	{
	case DEAD:
	{
		animation->SetPlay(type + 1);
		Vector2 pos = GetPosition();
		
		if (rotation.y == 0.0f)
			pos.x += DELTA * 150.0f;
		else
			pos.x -= DELTA * 150.0f;

		if (ctime < 0.1f)
			ctime += DELTA;
		else
			ctime = 0.1f;

		Color color = Color(1.0f, 1.0f, 1.0f, 1.0f);
		animation->UpdateColorBuffer(color, 4, 0.0f, 0.0f, 0.0f);

		if (ctime >= 0.1f)
			animation->UpdateColorBuffer(Color(), 0, 0, 0, 0);

		SetPosition(pos);

		animation->SetPosition(position);
		animation->SetRotation(-rotation);
		animation->SetScale(scale);
		animation->Update(V, P);

		if (!animation->IsPlay())
		{
			animation->SetStop();
			dead = true;
			return;
		}
		break;
	}
	case STOP:
	{
		animation->SetStop();
		break;
	}
	case MOVE:
	{
		animation->SetPlay(type);
		Player* pl = (Player*)OBJMANAGER->FindObject("Player");

		if (time < 5.0f)
			time += DELTA * 10.0f;
		else
			time = -5.0f;

		Vector2 pos1 = pl->GetPosition();
		Vector2 pos2 = GetPosition();

		float x = (pos1.x - pos2.x);
		float y = (pos1.y - pos2.y);
		float val = sqrtf(x * x + y * y);

		float angle = atan2((pos1.y - pos2.y), (pos1.x - pos2.x));

		if (x < 0.0f)
			SetRotation(0.0f, 0.0f, time);
		else
			SetRotation(0.0f, 180.0f, time);

		if (moving)
		{
			pos2.x += cosf(angle) * DELTA * moveSpeed;
			pos2.y += sinf(angle) * DELTA * moveSpeed;
		}

		SetPosition(pos2);

		static float ctime = 0.0f;

		if (ctime <= 0.0f && hitEffect->IsActive())
		{
			ctime = 0.125f;
		}

		if (ctime > 0.0f)
		{
			ctime -= DELTA;
			Color color = Color(1.0f, 1.0f, 1.0f, 1.0f);
			animation->UpdateColorBuffer(color, 4, 0.0f, 0.0f, 0.0f);
		}
		else
			animation->UpdateColorBuffer(Color(), 0, 0, 0, 0);


		animation->SetPosition(position);
		animation->SetRotation(rotation);
		animation->SetScale(scale);
		animation->UpdateColorBuffer(Color(), 0, 0, 0, 0);
		animation->Update(V, P);

		collider->SetPosition(animation->GetPosition());
		collider->SetRotation(animation->GetRotation());
		collider->SetScale(animation->GetTextureRealSize());

		collider->Update(V, P);

		break;
	}
	case FROZEN:
	{
		Player* pl = PLAYER;

		ftime += DELTA;

		if (ftime >= pl->GetScreenEffect()->GetFrozenTime())
			state = MOVE;
			
		animation->SetStop();
		animation->SetPosition(position);
		animation->SetScale(scale);
		animation->UpdateColorBuffer(Color(0.1f, 0.1f, 1.0f, 0.8f), 4, 0, 0, 0);
		animation->Update(V, P);
	}
	}

	hitEffect->SetPosition(position);
	hitEffect->SetScale(scale);
	hitEffect->Update(V, P); 
}

void Monster::Attacked(int id)
{
	EquipList* sl = (EquipList*)OBJMANAGER->FindObject("EquipList");
	Player* pl = PLAYER;

	float damage;
	
	switch (id)
	{
	case 8:
		damage = FLT_MAX;
		break;
	case 9:
		damage = 15.0f * (1 + pl->GetPower());
		break;
	default:
		damage = sl->GetSkillDamage(id) * (1 + pl->GetPower());
		pl->AddTotalDamage(id, damage);
		break;
	}

	hp -= damage;

	cout << damage << endl;

	if (hp <= 0.0f)
		state = DEAD;

	hitEffect->Reset();
}

RECT Monster::GetCollisionRect()
{
	RECT rect;

	if (state == DEAD || !intersect)
	{
		return { 100000000, 100000000, 100000000, 100000000 };
	}

	Vector2 pPos = GetPosition();
	Vector2 size = animation->GetTextureRealSize();

	rect.left = (LONG)(pPos.x - size.x / 2.5f);
	rect.right = (LONG)(pPos.x + size.x / 2.5f);
	rect.top = (LONG)(pPos.y + size.y / 2.5f);
	rect.bottom = (LONG)(pPos.y - size.y / 2.5f);

	return rect;
}


void Monster::Render()
{
	if (!IsActive())
		return;

	animation->Render();
	collider->Render();
	hitEffect->Render();
}

void Monster::Reset()
{
	hitEffect = (HitEffect*)OBJMANAGER->FindObject("HitEffect");

	if (!hitEffect->IsActive())
		hitEffect->Reset();

	state = MOVE;
	//type = NONE;

	dead = false;
	SetActive(true);
}

