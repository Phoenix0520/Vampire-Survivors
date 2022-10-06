#include "framework.h"
#include "ListCtrl.h"
#include "Cursor.h"
#include "Rect.h"

ListCtrl::ListCtrl()
{
	SetPosition(0.0f, -50.0f);
	Vector2 position = GetPosition();

	wstring imageFile1 = L"./Images/CollectionBox.png";
	wstring shaderFile = L"./Shader/HLSL/TextureColor.hlsl";

	wstring str = L"./Images/CollectionDialogue.png";
	cDialogue = new Texture(str, shaderFile);
	cDialogue->SetPosition(position.x, position.y-350.0f);


	str = L"./Images/Option.png";
	cOption = new Texture(str, shaderFile);
	cOption->SetPosition(position.x, position.y);

	rect = new Rect(L"./Shader/HLSL/Color.hlsl");

	texture = new Texture(L"./Images/CollectionBox.png", shaderFile);
	texture->SetOffset(0.0f, 0.0f);
	texture->SetOffsetSize(24.0f, 24.0f);
	texture->SetScale(2.875f * scale);

	SetProperty();


	wstring str2 = L"./Images/CollectionTexture/0.png";
	picture = new Texture(str2, shaderFile);


	// 커서
	{
		cursor = (Cursor*)OBJMANAGER->FindObject("Cursor");
	}
}

ListCtrl::~ListCtrl()
{
	SAFE_DELETE(cursor);
	SAFE_DELETE(picture);
	SAFE_DELETE(texture);
}

void ListCtrl::Update(Matrix V, Matrix P)
{
	if (!active)
		return;

	Vector2 pos = GetPosition();

	if (hover)
	{
		if (DOWN('S') || DOWN(VK_DOWN))
		{
			focusItem = focusItem + 8;
			if (focusItem > 55)
				focusItem = focusItem - 8;
		}
		if (DOWN('W') || DOWN(VK_UP))
		{
			focusItem = focusItem - 8;

			if (focusItem < 0)
			{
				focusItem = focusItem + 8;
				if (tabGroup[2])
				{
					focusItem = 0;
					SetHover(false);
					tabGroup[2]->SetHover(true);
				}
			}
		}
		if (DOWN('A') || DOWN(VK_LEFT))
		{
			if (focusItem == 0 || focusItem == 8 || focusItem == 16 || focusItem == 24 ||
				focusItem == 32 || focusItem == 40)
				focusItem = focusItem + 7;
			else
				focusItem--;
		}

		if (DOWN('D') || DOWN(VK_RIGHT))
		{
			if (focusItem == 15 || focusItem == 23 || focusItem == 31 || 
				focusItem == 39 || focusItem == 47)
				focusItem = focusItem - 7;
			else
				focusItem++;;
		}

		if (focusItem > 55)
			focusItem = 55;
	}

	texture->Update(V, P);

	if (!this->GetItemCollect(focusItem))
		cDialogue->UpdateColorBuffer(Color(0.5f, 0.5f, 0.5f, 1.0f), 9, 0, 0, 0);
	else
		cDialogue->UpdateColorBuffer(Color(), 0, 0, 0, 0);

	cDialogue->Update(V, P);
	cOption->Update(V, P);

	if (hover)
	{
		cursor->SetType(1);
		cursor->SetScale(2.5f, 2.5f);
		cursor->SetPosition(GetFocusItemPosition());
		cursor->Update(V, P);
	}
}

int ListCtrl::GetItemType(int id)
{
	ListItem *p = ListItemDatas[id];
	return(p->type);
}

bool ListCtrl::GetItemCollect(int id)
{
	ListItem *p = ListItemDatas[id];
	return(p->collect);
}

void ListCtrl::UpdateItemType(int id, int type)
{
	ListItem *p = ListItemDatas[id];
	p->type = type;
}

void ListCtrl::UpdateItemCollect(int id, bool collect)
{
	ListItem *p = ListItemDatas[id];
	p->collect = collect;
}

void ListCtrl::UpdateItemCollectEx(int id, bool val)
{
	int arrayID = 0;
	switch (id)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
		arrayID = id * 2;
		break;
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
		arrayID = id + 18;
		break;
	default:
		arrayID = -1;
		break;
	}

	if (arrayID == -1)
	{
		return;
	}

	ListItem *p = ListItemDatas[arrayID];
	p->collect = val;
}


void ListCtrl::AddListItem(int id,  wstring name, wstring explain1, wstring explain2, wstring tip)
{
	ListItem *pItem = new ListItem();
	pItem->id = id;
	pItem->type = 0;
	pItem->collect = false;
	pItem->name = name;
	pItem->explain1 = explain1;
	pItem->explain2 = explain2;
	pItem->tip = tip;
	ListItemDatas.push_back(pItem);

	wstring str = L"./Images/CollectionTexture/" + to_wstring(id) + L".png";
	SRVMANAGER->CreateShaderResourceView(str);
	Vector2 size = SRVMANAGER->FindSpriteSize(str);
	pItem->offset = Vector2(0.0f, 0.0f);
	pItem->offsetSize = size;
}

void ListCtrl::Render()
{
	if (!active)
		return;

	Vector2 position = GetPosition();

	CAMERA->Update();

	Matrix V = CAMERA->GetViewMatrix();
	Matrix P = CAMERA->GetProjMatrix();

	texture->SetOffset(0.0f, 0.0f);
	texture->Update(V, P);

	cOption->Render();
	cDialogue->Render();

	Vector2 pos = Vector2(position.x - 300.0f, position.y + 320.0f);

	// 박스 렌더링
	int ID = 0;
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (GetItemCollect(ID))
			{
				texture->SetScale(2.875f * scale);
				texture->UpdateColorBuffer(Color(), 0, 0, 0, 0);
			}
			else
			{
				texture->SetScale(2.25f * scale);
				texture->UpdateColorBuffer(Color(0.0f, 0.0, 0.0f, 0.5f), 7, 0, 0, 0);
			}
			
			if (GetItemType(ID) == 0)
				texture->SetOffset(0.0f, 0.0f);
			else
				texture->SetOffset(24.0f, 0.0f);

			texture->SetPosition(pos);
			texture->Update(V, P);
			texture->Render();

			pos.x = pos.x + 85.0f;
			ID++;
		}
		pos.x = position.x - 300.0f;
		pos.y = pos.y - 85.0f;

	}
	pos = Vector2(position.x - 300.0f, position.y + 320.0f);
	
	ID = 0;
	wstring str;

	// 그림 렌더링
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (GetItemCollect(ID))
			{
				str = L"./Images/CollectionTexture/" + to_wstring(ID) + L".png";

				picture->SetImageFile(str);
				picture->SetScale(2.875f * scale);
				picture->SetPosition(pos);

				picture->SetOffsetSize(SRVMANAGER->FindSpriteSize(str));
				picture->Update(V, P);

				picture->Render();
			}
			else
			{
				str = L"?";
				Vector2 pos2 = Vector2(pos.x - 10.0f, pos.y + 15.0f);
				CAMERA->VCToWC(pos2);

				DirectWrite::RenderText(str, pos2, 255, 255, 255, 20.0f);
			}

			pos.x = pos.x + 85.0f;
			ID++;
		}
		pos.x = position.x - 300.0f;
		pos.y = pos.y - 85.0f;
	}

	// 상세설명
	{
		pos = Vector2(position.x - 275.0f, position.y - 350.0f);
		texture->SetPosition(pos);

		if (GetItemType(focusItem) == 0)
			texture->SetOffset(0.0f, 0.0f);
		else
			texture->SetOffset(24.0f, 0.0f);

		if (GetItemCollect(focusItem))
		{
			texture->SetScale(2.875f * scale);
			texture->UpdateColorBuffer(Color(), 0, 0, 0, 0);
		}
		else
		{
			texture->SetScale(2.25f * scale);
			texture->UpdateColorBuffer(Color(0.0f, 0.0, 0.0f, 0.5f), 7, 0, 0, 0);
		}

		texture->Update(V, P);
		texture->Render();

		if (GetItemCollect(focusItem))
			picture->SetScale(2.85f, 2.85f);
		else
		{
			picture->SetScale(1.277777778f, 1.277777778f);

			str = L"?";
			Vector2 pos2 = Vector2(pos.x - 10.0f, pos.y + 15.0f);
			CAMERA->VCToWC(pos2);

			DirectWrite::RenderText(str, pos2, 255, 255, 255, 20.0f);
		}			

		str = L"./Images/CollectionTexture/" + to_wstring(focusItem) + L".png";
		picture->SetImageFile(str);
		picture->SetOffsetSize(SRVMANAGER->FindSpriteSize(str));
		picture->SetPosition(pos);

		picture->Update(V, P);

		if (GetItemCollect(focusItem))
			picture->Render();
	}

	// 컬렉션
	{
		int  num = 0;

		for (int i = 0; i < 56; i++)
		{
			if (GetItemCollect(i))
				num++;
		}

		wstring str = L"컬렉션 : " + to_wstring(num) + L" / 56";
		wstring script1;
		wstring script2;
		wstring script3;

		float x = -(str.length() * 45.0f * 0.5f) + GetPosition().x;
		float y = 0.0f;

		Vector2 pos = Vector2(x, 425.0f + GetPosition().y);
		CAMERA->VCToWC(pos);

		DirectWrite::RenderText(str, pos, 255, 255, 255, 45.0f);
	}

	// Text Write
	{
		ListItem *p = ListItemDatas[focusItem];

		wstring str = p->name;
		wstring script1 = p->explain1;
		wstring script2 = p->explain2;
		wstring script3 = p->tip;

		if (!GetItemCollect(focusItem))
		{
			str = L"???";
			script1 = L"아직 발견하지 못했습니다.";
			script2 = L"";
			script3 = L"";
		}
		
		{
			float x = -(str.length() * 20.0f * 0.5f) + GetPosition().x;
			float y = -335.0f;
			pos = Vector2(x, y);

			CAMERA->VCToWC(pos);

			DirectWrite::RenderText(str, pos, 255, 255, 0, 20.0f);

			x = -(script1.length() * 20.0f * 0.5f) + GetPosition().x;
			y = -365.0f;
			pos = Vector2(x, y);

			CAMERA->VCToWC(pos);

			DirectWrite::RenderText(script1, pos, 255, 255, 255, 20.0f);

			x = -(script2.length() * 20.0f * 0.5f) + GetPosition().x;
			y = -395.0f;
			pos = Vector2(x, y);

			CAMERA->VCToWC(pos);

			DirectWrite::RenderText(script2, pos, 255, 255, 255, 20.0f);

			x = -(script3.length() * 20.0f * 0.5f) + GetPosition().x;
			y = -435.0f;
			pos = Vector2(x, y);

			CAMERA->VCToWC(pos);
			DirectWrite::RenderText(script3, pos, 255, 165, 0, 20.0f);
		}
	}

	if (hover)
		cursor->Render();

}

void ListCtrl::Reset()
{
	hover = false;
}

void ListCtrl::SetProperty()
// Name, Explain Set
{
	wstring  strName;
	wstring  strTip;
	wstring  strExplain1;
	wstring  strExplain2;

	strName     = L"채찍";
	strExplain1 = L"수평으로 적을 관통해 공격합니다.";
	strExplain2 = L"";
	strTip      = L"파워업 무시 : 지속시간";
	AddListItem(0,  strName, strExplain1, strExplain2, strTip);

	strName = L"???";
	strExplain1 = L"아직 발견하지 못했습니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(1, strName, strExplain1, strExplain2, strTip);

	strName = L"매직 완드";
	strExplain1 = L"가장 가까운 적을 공격합니다.";
	strExplain2 = L"";
	strTip = L"파워업 무시 : 지속시간";
	AddListItem(2, strName, strExplain1, strExplain2, strTip);

	strName = L"???";
	strExplain1 = L"아직 발견하지 못했습니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(3, strName, strExplain1, strExplain2, strTip);

	strName = L"단검";
	strExplain1 = L"보고 있는 방향으로 빠르게 발사됩니다.";
	strExplain2 = L"";
	strTip = L"파워업 무시 : 지속시간";
	AddListItem(4, strName, strExplain1, strExplain2, strTip);

	strName = L"???";
	strExplain1 = L"아직 발견하지 못했습니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(5, strName, strExplain1, strExplain2, strTip);

	strName     = L"도끼";
	strExplain1 = L"피해량이 높고 공격 범위가 넓습니다.";
	strExplain2 = L"";
	strTip = L"파워업 무시 : 지속시간";
	AddListItem(6, strName, strExplain1, strExplain2, strTip);

	strName = L"???";
	strExplain1 = L"아직 발견하지 못했습니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(7, strName, strExplain1, strExplain2, strTip);

	strName     = L"십자가";
	strExplain1 = L"가장 가까운 적에게 날아가며 부메랑처럼";
	strExplain2 = L"돌아옵니다.";
	strTip      = L"파워업 무시 : 지속시간";
	AddListItem(8, strName, strExplain1, strExplain2, strTip);

	strName = L"???";
	strExplain1 = L"아직 발견하지 못했습니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(9, strName, strExplain1, strExplain2, strTip);

	strName = L"기도문";
	strExplain1 = L"기도문이 주변을 회전하며 공격합니다.";
	strExplain2 = L"";
	strTip = L"파워업 무시 : 투사체 속도, 지속시간, 공격범위";
	AddListItem(10, strName, strExplain1, strExplain2, strTip);

	strName = L"???";
	strExplain1 = L"아직 발견하지 못했습니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(11, strName, strExplain1, strExplain2, strTip);

	strName = L"파이어 완드";
	strExplain1 = L"무작위 적을 향해 발사되며 큰 피해를 줍니다.";
	strExplain2 = L"";
	strTip = L"파워업 무시 : 지속시간";
	AddListItem(12, strName, strExplain1, strExplain2, strTip);

	strName = L"???";
	strExplain1 = L"아직 발견하지 못했습니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(13, strName, strExplain1, strExplain2, strTip);


	strName = L"마늘";
	strExplain1 = L"범위 내에 적에게 피해를 줍니다. 넉백과";
	strExplain2 = L"빙결저항을 감소시킵니다.";
	strTip = L"파워업 무시 : 투사체 수, 지속시간, 투사체 속도";
	AddListItem(14, strName, strExplain1, strExplain2, strTip);

	strName = L"???";
	strExplain1 = L"아직 발견하지 못했습니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(15, strName, strExplain1, strExplain2, strTip);

	strName =   L"성수";
	strExplain1    = L"화염 구덩이를 생성합니다.";
	strExplain2 = L"";
	strTip = L"파워업 무시 : 투사체 속도";
	AddListItem(16, strName, strExplain1, strExplain2, strTip);

	strName = L"???";
	strExplain1 = L"아직 발견하지 못했습니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(17, strName, strExplain1, strExplain2, strTip);

	strName =   L"룬트레이서";
	strExplain1    = L"적을 관통하며 튕겨져 나옵니다.";
	strExplain2 = L"";
	strTip = L"추천 조합 : 투사체 속도, 지속시간";
	AddListItem(18, strName, strExplain1, strExplain2, strTip);

	strName = L"???";
	strExplain1 = L"아직 발견하지 못했습니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(19, strName, strExplain1, strExplain2, strTip);

	strName =   L"번개 반지";
	strExplain1    = L"무작위 적에게 번개를 내리칩니다.";
	strExplain2 = L"";
	strTip = L"파워업 무시	: 투사체 속도, 지속시간";
	AddListItem(20, strName, strExplain1, strExplain2, strTip);

	strName = L"???";
	strExplain1 = L"아직 발견하지 못했습니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(21, strName, strExplain1, strExplain2, strTip);

	strName =   L"하얀 비둘기";
	strExplain1    = L"원형의 구역을 폭격합니다.";
	strExplain2 = L"";
	strTip = L"추천 조합 : 쿨타임, 지속시간";
	AddListItem(22, strName, strExplain1, strExplain2, strTip);
	
	
	strName = L"???";
	strExplain1 = L"아직 발견하지 못했습니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(23, strName, strExplain1, strExplain2, strTip);

	strName =   L"시계바늘";
	strExplain1    = L"적들을 잠시 얼려 움직이지 못하게 합니다.";
	strExplain2 = L"";
	strTip = L"파워업 무시 : 괴력, 투사체 속도, 투사체 수, 공격 범위";
	AddListItem(24, strName, strExplain1, strExplain2, strTip);

	strName =   L"월계수";
	strExplain1    = L"피격 데미지를 막아줍니다.";
	strExplain2 = L"";
	strTip = L"추천 조합 : 쿨타임만 적용됨";
	AddListItem(25, strName, strExplain1, strExplain2, strTip);


	strName =   L"시금치";
	strExplain1    = L"피해량이 10% 증가합니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(26, strName, strExplain1, strExplain2, strTip);

	strName =   L"갑옷";
	strExplain1    = L"피격 데미지를 1 감소시킵니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(27, strName, strExplain1, strExplain2, strTip);

	strName =   L"칠흑 심장";
	strExplain1    = L"최대 체력이 20% 증가합니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(28, strName, strExplain1, strExplain2, strTip);

	strName =   L"붉은 심장";
	strExplain1    = L"초당 체력 재생이 0.2 증가합니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(29, strName, strExplain1, strExplain2, strTip);

	strName =   L"빈 성서";
	strExplain1    = L"쿨타임이 8% 감소합니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(30, strName, strExplain1, strExplain2, strTip);

	strName =   L"촛대";
	strExplain1    = L"공격 범위가 10% 증가합니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(31, strName, strExplain1, strExplain2, strTip);

	strName =   L"브레이서";
	strExplain1    = L"투사체 속도가 10% 증가합니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(32, strName, strExplain1, strExplain2, strTip);

	strName =   L"주문속박기";
	strExplain1    = L"쿨타임이 10% 증가합니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(33, strName, strExplain1, strExplain2, strTip);

	strName =   L"복제 반지";
	strExplain1    = L"투사체 수가 1 증가합니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(34, strName, strExplain1, strExplain2, strTip);

	strName =   L"날개";
	strExplain1    = L"10% 빠르게 움직입니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(35, strName, strExplain1, strExplain2, strTip);

	strName =   L"매혹의 오브";
	strExplain1    = L"더 멀리 있는 아이템을 획득합니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(36, strName, strExplain1, strExplain2, strTip);

	strName =   L"클로버";
	strExplain1    = L"행운이 10% 증가합니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(37, strName, strExplain1, strExplain2, strTip);

	strName =   L"왕관";
	strExplain1    = L"경험치 획득량이 8% 증가합니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(38, strName, strExplain1, strExplain2, strTip);

	for (int i = 39; i < 45; i++)
	{
		strName = L"???";
		strExplain1 = L"아직 발견하지 못했습니다.";
		strExplain2 = L"";
		strTip = L"";
		AddListItem(39, strName, strExplain1, strExplain2, strTip);
	}

	strName =   L"경험치 보석";
	strExplain1    = L"경험치를 증가시킵니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(45, strName, strExplain1, strExplain2, strTip);

	strName =   L"금화";
	strExplain1    = L"1골드를 추가합니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(46, strName, strExplain1, strExplain2, strTip);

	strName =   L"동전 가방";
	strExplain1    = L"10골드를 추가합니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(47, strName, strExplain1, strExplain2, strTip);

	strName =   L"묵직한 동전 가방";
	strExplain1    = L"100골드를 추가합니다.";
	strExplain2 = L"";
	strTip = L"행운이 높을수록 드랍될 확률이 높아집니다.";
	AddListItem(48, strName, strExplain1, strExplain2, strTip);

	strName =   L"묵주";
	strExplain1    = L"시야 내의 모든 적을 파괴합니다.";
	strExplain2 = L"";
	strTip = L"행운이 높을수록 드랍될 확률이 높아집니다.";
	AddListItem(49, strName, strExplain1, strExplain2, strTip);

	strName =   L"은두자 프리타";
	strExplain1    = L"바라보는 방향으로 화염을 발사합니다. 플레이어";
	strExplain2    = L"스탯에 영향을 받습니다.";
	strTip = L"행운이 높을수록 드랍될 확률이 높아집니다.";
	AddListItem(50, strName, strExplain1, strExplain2, strTip);

	strName =   L"회중시계";
	strExplain1    = L"10초간 모든 적을 얼려버립니다.";
	strExplain2 = L"";
	strTip = L"행운이 높을수록 드랍될 확률이 높아집니다.";
	AddListItem(51, strName, strExplain1, strExplain2, strTip);

	strName =   L"흡입기";
	strExplain1    = L"지면의 모든 경험치 보석을 모아줍니다.";
	strExplain2 = L"";
	strTip = L"행운이 높을수록 드랍될 확률이 높아집니다.";
	AddListItem(52, strName, strExplain1, strExplain2, strTip);

	strName =   L"치킨";
	strExplain1    = L"체력을 30 회복합니다.";
	strExplain2 = L"";
	strTip = L"행운이 높을수록 드랍될 확률이 높아집니다.";
	AddListItem(53, strName, strExplain1, strExplain2, strTip);

	strName =   L"작은 클로버";
	strExplain1    = L"획득시 행운이 10% 증가합니다.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(54, strName, strExplain1, strExplain2, strTip);

	strName =   L"보물상자";
	strExplain1    = L"골드와 무작위 파워 업을 획득합니다. 강력한";
	strExplain2    = L"적에게서만 획득할 수 있습니다.";
	strTip = L"행운이 높을수록 아이템가 골드 획득량이 높아집니다.";
	AddListItem(55, strName, strExplain1, strExplain2, strTip);

	for (int i = 0; i < 56; i++)
	{
		UpdateItemType(i, 0);

		if (i >= 45)
		{
			if (i == 45 || i == 46)
			{
				UpdateItemCollect(i, true);
			}
			UpdateItemType(i, 1);
		}
	} 
}

Vector2 ListCtrl::GetFocusItemPosition()
{
	int  x = (int)((focusItem) % 8);
	int  y = (int)((focusItem) / 8);

	Vector2  position = GetPosition();
	Vector2  pos = Vector2(position.x - 300.0f, position.y + 320.0f);

	pos.x = pos.x + x * 85.0f;
	pos.y = pos.y - y * 85.0f;
	return pos;
}
