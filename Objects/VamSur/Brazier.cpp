#include "framework.h"
#include "Brazier.h"
#include "Player.h"
#include "DropItem.h"

Brazier::Brazier()
{
	wstring imageFile1 = L"./Images/Brazier.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	texture = new Texture(imageFile1, shaderFile);
	texture->SetScale(2.15f, 2.15f);

	SetActive(false);
}

Brazier::~Brazier()
{
	SAFE_DELETE(texture);
}

void Brazier::Update(Matrix V, Matrix P)
{
	if (!IsActive())
		return;

	if (hp <= 0.0f)
	{
		DropItem* item = (DropItem*)OBJMANAGER->FindObject("DropItem" + to_string(id));

		item->Reset();
		item->SetType((DropItem::Type)include);

		cout << include << " 번째 타입 아이템 생성됨" << endl;

		SetActive(false);
	}

	if (underAttack)
	{
		time -= DELTA;

		texture->UpdateColorBuffer(Color(0, 0, 0, time + 0.25f), 4, 0, 0, 0);

		if (time < 0.0f)
			underAttack = false;
	}
	else
		texture->UpdateColorBuffer(Color(), 0, 0, 0, 0);

	texture->SetPosition(GetPosition());
	texture->Update(V, P);
}

void Brazier::Render()
{
	if (!IsActive())
		return;
	texture->Render();
}

void Brazier::Reset()
{
	SetActive(true);

	mt19937 engine((UINT)GetTickCount());
	uniform_int_distribution<> distributon1(0, 1000);
	auto generator1 = bind(distributon1, engine);
	
	Player* pl = PLAYER;

	float value = (float)generator1() / 10.0f;
	float luck = pl->GetLuck() * 3.0f;

	if (value <= 100.0f - luck)
		include = Clover;
	else if (value <= 97.5f - luck)
		include = Vaccum;
	else if (value <= 95.0f - luck)
		include = Clock;
	else if (value <= 92.5f - luck)
		include = Nduja;
	else if (value <= 90.0f - luck)
		include = Rosary;
	else if (value <= 87.5f - luck)
		include = Chicken;
	else if (value <= 85.0f - luck)
		include = BigCoinBag;
	else if (value <= 82.5f - luck)
		include = CoinBag;
	else
		include = Coin;

	uniform_int_distribution<> distributon2(-300, 300);
	auto generator2 = bind(distributon2, engine);

	Vector2 pos = Vector2((float)generator2(), (float)generator2());
	cout << pos.x << " , " << pos.y << " 위치에 ";

	SetPosition(pos);

	hp = 10.0f;

	id = 0;

	for (int i = 0; i < MAX_ITEM; i++)
	{
		DropItem* item = (DropItem*)OBJMANAGER->FindObject("DropItem" + to_string(i));

		if (item->IsActive())
		{
			id++;
			continue;
		}
		else
		{
			item->SetPosition(GetPosition());
			item->SetActive(true);
			break;
		}
	}

	cout << id << endl;
}

void Brazier::Attacked(float damage)
{
	if (!underAttack)
		hp -= damage;

	time = 0.5f;

	underAttack = true;
}
