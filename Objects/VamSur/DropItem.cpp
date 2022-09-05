#include "framework.h"
#include "DropItem.h"
#include "Player.h"
#include "Cursor.h"

DropItem::DropItem()
{
	wstring imageFile1 = L"./Images/InGameSceneTexture/DropItems.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	texture = new Texture(imageFile1, shaderFile);

	animation = new Animation(imageFile1, shaderFile);

	float x = 0.0f;
	// Rosary
	{
		AnimationClip* clip = new AnimationClip(AnimationClip::loop);
		animation->AddClip(clip);

		for (int i = 0; i < 3; i++)
		{
			clip->AddFrame(animation->GetTexture(), imageFile1, x, 15.0f, x + 16.0f, 31.0f, 0.15f);
			x += 16.0f;
		}
	}

	x = 0.0f;

	// Nduja
	{
		AnimationClip* clip = new AnimationClip(AnimationClip::loop);
		animation->AddClip(clip);

		for (int i = 0; i < 3; i++)
		{
			clip->AddFrame(animation->GetTexture(), imageFile1, x, 31.0f, x + 15.0f, 45.0f, 0.15f);
			x += 15.0f;
		}
	}

	x = 0.0f;

	// Clock
	{
		AnimationClip* clip = new AnimationClip(AnimationClip::loop);
		animation->AddClip(clip);

		for (int i = 0; i < 3; i++)
		{
			clip->AddFrame(animation->GetTexture(), imageFile1, x, 45.0f, x + 16.0f, 61.0f, 0.15f);
			x += 16.0f;
		}
	}

	x = 0.0f;

	// Vaccum
	{
		AnimationClip* clip = new AnimationClip(AnimationClip::loop);
		animation->AddClip(clip);

		for (int i = 0; i < 3; i++)
		{
			clip->AddFrame(animation->GetTexture(), imageFile1, x, 61.0f, x + 16.0f, 77.0f, 0.15f);
			x += 16.0f;
		}
	}

	texture->SetScale(2.25f, 2.25f);
	animation->SetScale(2.25f, 2.25f);
	
	player = PLAYER;

	collider = new Collider();
	playerCenterDot = new Collider();
}

DropItem::~DropItem()
{
	SAFE_DELETE(animation);
	SAFE_DELETE(texture);
}

void DropItem::Update(Matrix V, Matrix P)
{
	switch (type)
	{
	case DropItem::NONE:
		SetActive(false);
		break;
	case DropItem::EXP:
		texture->SetOffset(0.0f, 0.0f);
		texture->SetOffsetSize(9.0f, 12.0f);
		texture->Update(V, P);
		break;
	case DropItem::Coin:
		texture->SetOffset(9.0f, 0.0f);
		texture->SetOffsetSize(10.0f, 10.0f);
		texture->Update(V, P);
		break;
	case DropItem::CoinBag:
		texture->SetOffset(19.0f, 0.0f);
		texture->SetOffsetSize(12.0f, 14.0f);
		texture->Update(V, P);
		break;
	case DropItem::BigCoinBag:
		texture->SetOffset(31.0f, 0.0f);
		texture->SetOffsetSize(16.0f, 15.0f);
		texture->Update(V, P);
		break;
	case DropItem::Chicken:
		texture->SetOffset(47.0f, 0.0f);
		texture->SetOffsetSize(16.0f, 14.0f);
		texture->Update(V, P);
		break;
	case DropItem::Rosary:
		animation->SetPlay(0);
		animation->Update(V, P);
		break;
	case DropItem::Nduja:
		animation->SetPlay(1);
		animation->Update(V, P);
		break;
	case DropItem::Clock:
		animation->SetPlay(2);
		animation->Update(V, P);
		break;
	case DropItem::Vaccum:
		animation->SetPlay(3);
		animation->Update(V, P);
		break;
	case DropItem::Clover:
		texture->SetOffset(63.0f, 0.0f);
		texture->SetOffsetSize(15.0f, 14.0f);
		texture->Update(V, P);
		break;
	case DropItem::Chest:
		if (arrow == nullptr)
			arrow = (Cursor*)OBJMANAGER->FindObject("ChestCursor");
		arrow->SetType(3);
		arrow->SetPosition(GetPosition());
		arrow->Update(V, P);
		texture->SetOffset(78.0f, 0.0f);
		texture->SetOffsetSize(14.0f, 12.0f);
		texture->Update(V, P);
		break;
	}

	if (!IsActive())
		return;

	texture->SetPosition(GetPosition());
	animation->SetPosition(GetPosition());
	
	collider->SetPosition(GetPosition());
	collider->SetRotation(GetRotation());

	if (type == Rosary || type == Nduja || type == Clock || type == Vaccum)
		collider->SetScale(animation->GetTextureRealSize());
	else
		collider->SetScale(texture->GetTextureRealSize());

	collider->Update(V, P);

	playerCenterDot->SetPosition(player->GetPosition());
	playerCenterDot->SetScale(5.0f, 5.0f);
	playerCenterDot->Update(V, P);

	{
		if (!gettering)
		{
			if (type == EXP)
			{
				if (IsPlayerInRegion(player->GetMagnet()))
					gettering = true;
			}
			else if (type == Rosary || type == Nduja || type == Clock || type == Vaccum)
			{
				if (IsPlayerInRegionEx(60.0f))
					gettering = true;
			}
			else if (type == Chest)
			{
				if (IsPlayerInRegion())
				{
					arrow->SetType(0);
					used = true;
				}
			}
			else
				if (IsPlayerInRegion(60.0f))
					gettering = true;
		}

		pos1 = this->GetPosition();
		pos2 = player->GetPosition();

		angle = atan2(pos1.y - pos2.y, pos1.x - pos2.x);

		if (gettering)
		{
			float mSpeed = 250.0f;

			time += DELTA;

			if (time <= 0.3f)
			{
				pos1.x += cosf(angle) * DELTA * mSpeed * (1.0f + (-time * 3.3f + 1.0f));
				pos1.y += sinf(angle) * DELTA * mSpeed * (1.0f + (-time * 3.3f + 1.0f));
			}
			else
			{
				if (Collider::InterSect(collider, playerCenterDot))
				{
					used = true;
					gettering = false;
				}

				pos1.x -= cosf(angle) * DELTA * mSpeed * (1.0f + (time - 0.3f));
				pos1.y -= sinf(angle) * DELTA * mSpeed * (1.0f + (time - 0.3f));
			}

			SetPosition(pos1);
		}

		if (used)
			UsingItem();
	}
}

void DropItem::Render()
{
	if (!IsActive())
		return;

	switch (type)
	{
	case DropItem::NONE:
	case DropItem::EXP:
	case DropItem::Coin:
	case DropItem::CoinBag:
	case DropItem::BigCoinBag:
	case DropItem::Chicken:
	case DropItem::Clover:
		texture->Render();
		break;
	case DropItem::Rosary:
	case DropItem::Nduja:
	case DropItem::Clock:
	case DropItem::Vaccum:
		animation->Render();
		break;	
	case DropItem::Chest:
		texture->Render();
		arrow->Render();
		break;
	}

	collider->Render();
}

void DropItem::Reset()
{
	time = 0.0f;
	angle = 0.0f;
	expVal = 10.0f;
	used = false;
	gettering = false;
	SetActive(true);
}

void DropItem::UsingItem()
{
	SetActive(false);
	time = 0.0f;

	player->SetMagnet(30.0f);

	switch (type)
	{
	case DropItem::EXP:
		player->AddExp(expVal);
		break;
	case DropItem::Coin:
		player->SetCoin(player->GetCoin() + 1);
		break;
	case DropItem::CoinBag:
		player->SetCoin(player->GetCoin() + 10);
		break;
	case DropItem::BigCoinBag:
		player->SetCoin(player->GetCoin() + 100);
		break;
	case DropItem::Chicken:
		player->EatChicken();
		break;
	case DropItem::Rosary:
		player->UseRosary();
		break;
	case DropItem::Nduja:
		player->UseNduja();
		break;
	case DropItem::Clock:
		player->UseClock();
		break;
	case DropItem::Vaccum:
		player->SetMagnet(FLT_MAX);
		break;
	case DropItem::Clover:
		player->SetLuck(player->GetLuck() + 0.1f);
		break;
	case DropItem::Chest:
		player->UseChest();
		break;
	default:
		break;
	}

	static const char* items[] =
	{
		"NONE", "EXP", "Coin", "CoinBag", "BigCoinBag", "Chicken",
		"Rosary", "Nduja", "Clock", "Vaccum", "Clover", "Chest"
	};

	cout << "Item " << items[type + 1] << " È¹µæ!" << endl;
}

bool DropItem::IsPlayerInRegion(float adv)
{
	if (type < 0)
		return false;

	Vector2 tPos = texture->GetPosition();
	Vector2 pPos = player->GetPosition();

	float dx = pPos.x - tPos.x;
	float dy = pPos.y - tPos.y;

	double distance = sqrt(dx * dx + dy * dy);

	float size = texture->GetTextureRealSize().x;

	if (distance <= adv + size)
		return true;
	else
		return false;
}

bool DropItem::IsPlayerInRegionEx(float adv)
{
	if (type <= 0)
		return false;

	Vector2 tPos = animation->GetPosition();
	Vector2 pPos = player->GetPosition();

	float dx = pPos.x - tPos.x;
	float dy = pPos.y - tPos.y;

	double distance = sqrt(dx * dx + dy * dy);

	float size = animation->GetTextureRealSize().x;

	if (distance <= adv + size)
		return true;
	else
		return false;
}
