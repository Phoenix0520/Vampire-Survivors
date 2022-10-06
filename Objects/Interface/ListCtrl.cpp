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


	// Ŀ��
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

	// �ڽ� ������
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

	// �׸� ������
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

	// �󼼼���
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

	// �÷���
	{
		int  num = 0;

		for (int i = 0; i < 56; i++)
		{
			if (GetItemCollect(i))
				num++;
		}

		wstring str = L"�÷��� : " + to_wstring(num) + L" / 56";
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
			script1 = L"���� �߰����� ���߽��ϴ�.";
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

	strName     = L"ä��";
	strExplain1 = L"�������� ���� ������ �����մϴ�.";
	strExplain2 = L"";
	strTip      = L"�Ŀ��� ���� : ���ӽð�";
	AddListItem(0,  strName, strExplain1, strExplain2, strTip);

	strName = L"???";
	strExplain1 = L"���� �߰����� ���߽��ϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(1, strName, strExplain1, strExplain2, strTip);

	strName = L"���� �ϵ�";
	strExplain1 = L"���� ����� ���� �����մϴ�.";
	strExplain2 = L"";
	strTip = L"�Ŀ��� ���� : ���ӽð�";
	AddListItem(2, strName, strExplain1, strExplain2, strTip);

	strName = L"???";
	strExplain1 = L"���� �߰����� ���߽��ϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(3, strName, strExplain1, strExplain2, strTip);

	strName = L"�ܰ�";
	strExplain1 = L"���� �ִ� �������� ������ �߻�˴ϴ�.";
	strExplain2 = L"";
	strTip = L"�Ŀ��� ���� : ���ӽð�";
	AddListItem(4, strName, strExplain1, strExplain2, strTip);

	strName = L"???";
	strExplain1 = L"���� �߰����� ���߽��ϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(5, strName, strExplain1, strExplain2, strTip);

	strName     = L"����";
	strExplain1 = L"���ط��� ���� ���� ������ �н��ϴ�.";
	strExplain2 = L"";
	strTip = L"�Ŀ��� ���� : ���ӽð�";
	AddListItem(6, strName, strExplain1, strExplain2, strTip);

	strName = L"???";
	strExplain1 = L"���� �߰����� ���߽��ϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(7, strName, strExplain1, strExplain2, strTip);

	strName     = L"���ڰ�";
	strExplain1 = L"���� ����� ������ ���ư��� �θ޶�ó��";
	strExplain2 = L"���ƿɴϴ�.";
	strTip      = L"�Ŀ��� ���� : ���ӽð�";
	AddListItem(8, strName, strExplain1, strExplain2, strTip);

	strName = L"???";
	strExplain1 = L"���� �߰����� ���߽��ϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(9, strName, strExplain1, strExplain2, strTip);

	strName = L"�⵵��";
	strExplain1 = L"�⵵���� �ֺ��� ȸ���ϸ� �����մϴ�.";
	strExplain2 = L"";
	strTip = L"�Ŀ��� ���� : ����ü �ӵ�, ���ӽð�, ���ݹ���";
	AddListItem(10, strName, strExplain1, strExplain2, strTip);

	strName = L"???";
	strExplain1 = L"���� �߰����� ���߽��ϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(11, strName, strExplain1, strExplain2, strTip);

	strName = L"���̾� �ϵ�";
	strExplain1 = L"������ ���� ���� �߻�Ǹ� ū ���ظ� �ݴϴ�.";
	strExplain2 = L"";
	strTip = L"�Ŀ��� ���� : ���ӽð�";
	AddListItem(12, strName, strExplain1, strExplain2, strTip);

	strName = L"???";
	strExplain1 = L"���� �߰����� ���߽��ϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(13, strName, strExplain1, strExplain2, strTip);


	strName = L"����";
	strExplain1 = L"���� ���� ������ ���ظ� �ݴϴ�. �˹��";
	strExplain2 = L"���������� ���ҽ�ŵ�ϴ�.";
	strTip = L"�Ŀ��� ���� : ����ü ��, ���ӽð�, ����ü �ӵ�";
	AddListItem(14, strName, strExplain1, strExplain2, strTip);

	strName = L"???";
	strExplain1 = L"���� �߰����� ���߽��ϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(15, strName, strExplain1, strExplain2, strTip);

	strName =   L"����";
	strExplain1    = L"ȭ�� �����̸� �����մϴ�.";
	strExplain2 = L"";
	strTip = L"�Ŀ��� ���� : ����ü �ӵ�";
	AddListItem(16, strName, strExplain1, strExplain2, strTip);

	strName = L"???";
	strExplain1 = L"���� �߰����� ���߽��ϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(17, strName, strExplain1, strExplain2, strTip);

	strName =   L"��Ʈ���̼�";
	strExplain1    = L"���� �����ϸ� ƨ���� ���ɴϴ�.";
	strExplain2 = L"";
	strTip = L"��õ ���� : ����ü �ӵ�, ���ӽð�";
	AddListItem(18, strName, strExplain1, strExplain2, strTip);

	strName = L"???";
	strExplain1 = L"���� �߰����� ���߽��ϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(19, strName, strExplain1, strExplain2, strTip);

	strName =   L"���� ����";
	strExplain1    = L"������ ������ ������ ����Ĩ�ϴ�.";
	strExplain2 = L"";
	strTip = L"�Ŀ��� ����	: ����ü �ӵ�, ���ӽð�";
	AddListItem(20, strName, strExplain1, strExplain2, strTip);

	strName = L"???";
	strExplain1 = L"���� �߰����� ���߽��ϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(21, strName, strExplain1, strExplain2, strTip);

	strName =   L"�Ͼ� ��ѱ�";
	strExplain1    = L"������ ������ �����մϴ�.";
	strExplain2 = L"";
	strTip = L"��õ ���� : ��Ÿ��, ���ӽð�";
	AddListItem(22, strName, strExplain1, strExplain2, strTip);
	
	
	strName = L"???";
	strExplain1 = L"���� �߰����� ���߽��ϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(23, strName, strExplain1, strExplain2, strTip);

	strName =   L"�ð�ٴ�";
	strExplain1    = L"������ ��� ��� �������� ���ϰ� �մϴ�.";
	strExplain2 = L"";
	strTip = L"�Ŀ��� ���� : ����, ����ü �ӵ�, ����ü ��, ���� ����";
	AddListItem(24, strName, strExplain1, strExplain2, strTip);

	strName =   L"�����";
	strExplain1    = L"�ǰ� �������� �����ݴϴ�.";
	strExplain2 = L"";
	strTip = L"��õ ���� : ��Ÿ�Ӹ� �����";
	AddListItem(25, strName, strExplain1, strExplain2, strTip);


	strName =   L"�ñ�ġ";
	strExplain1    = L"���ط��� 10% �����մϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(26, strName, strExplain1, strExplain2, strTip);

	strName =   L"����";
	strExplain1    = L"�ǰ� �������� 1 ���ҽ�ŵ�ϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(27, strName, strExplain1, strExplain2, strTip);

	strName =   L"ĥ�� ����";
	strExplain1    = L"�ִ� ü���� 20% �����մϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(28, strName, strExplain1, strExplain2, strTip);

	strName =   L"���� ����";
	strExplain1    = L"�ʴ� ü�� ����� 0.2 �����մϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(29, strName, strExplain1, strExplain2, strTip);

	strName =   L"�� ����";
	strExplain1    = L"��Ÿ���� 8% �����մϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(30, strName, strExplain1, strExplain2, strTip);

	strName =   L"�д�";
	strExplain1    = L"���� ������ 10% �����մϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(31, strName, strExplain1, strExplain2, strTip);

	strName =   L"�극�̼�";
	strExplain1    = L"����ü �ӵ��� 10% �����մϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(32, strName, strExplain1, strExplain2, strTip);

	strName =   L"�ֹ��ӹڱ�";
	strExplain1    = L"��Ÿ���� 10% �����մϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(33, strName, strExplain1, strExplain2, strTip);

	strName =   L"���� ����";
	strExplain1    = L"����ü ���� 1 �����մϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(34, strName, strExplain1, strExplain2, strTip);

	strName =   L"����";
	strExplain1    = L"10% ������ �����Դϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(35, strName, strExplain1, strExplain2, strTip);

	strName =   L"��Ȥ�� ����";
	strExplain1    = L"�� �ָ� �ִ� �������� ȹ���մϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(36, strName, strExplain1, strExplain2, strTip);

	strName =   L"Ŭ�ι�";
	strExplain1    = L"����� 10% �����մϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(37, strName, strExplain1, strExplain2, strTip);

	strName =   L"�հ�";
	strExplain1    = L"����ġ ȹ�淮�� 8% �����մϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(38, strName, strExplain1, strExplain2, strTip);

	for (int i = 39; i < 45; i++)
	{
		strName = L"???";
		strExplain1 = L"���� �߰����� ���߽��ϴ�.";
		strExplain2 = L"";
		strTip = L"";
		AddListItem(39, strName, strExplain1, strExplain2, strTip);
	}

	strName =   L"����ġ ����";
	strExplain1    = L"����ġ�� ������ŵ�ϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(45, strName, strExplain1, strExplain2, strTip);

	strName =   L"��ȭ";
	strExplain1    = L"1��带 �߰��մϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(46, strName, strExplain1, strExplain2, strTip);

	strName =   L"���� ����";
	strExplain1    = L"10��带 �߰��մϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(47, strName, strExplain1, strExplain2, strTip);

	strName =   L"������ ���� ����";
	strExplain1    = L"100��带 �߰��մϴ�.";
	strExplain2 = L"";
	strTip = L"����� �������� ����� Ȯ���� �������ϴ�.";
	AddListItem(48, strName, strExplain1, strExplain2, strTip);

	strName =   L"����";
	strExplain1    = L"�þ� ���� ��� ���� �ı��մϴ�.";
	strExplain2 = L"";
	strTip = L"����� �������� ����� Ȯ���� �������ϴ�.";
	AddListItem(49, strName, strExplain1, strExplain2, strTip);

	strName =   L"������ ����Ÿ";
	strExplain1    = L"�ٶ󺸴� �������� ȭ���� �߻��մϴ�. �÷��̾�";
	strExplain2    = L"���ȿ� ������ �޽��ϴ�.";
	strTip = L"����� �������� ����� Ȯ���� �������ϴ�.";
	AddListItem(50, strName, strExplain1, strExplain2, strTip);

	strName =   L"ȸ�߽ð�";
	strExplain1    = L"10�ʰ� ��� ���� ��������ϴ�.";
	strExplain2 = L"";
	strTip = L"����� �������� ����� Ȯ���� �������ϴ�.";
	AddListItem(51, strName, strExplain1, strExplain2, strTip);

	strName =   L"���Ա�";
	strExplain1    = L"������ ��� ����ġ ������ ����ݴϴ�.";
	strExplain2 = L"";
	strTip = L"����� �������� ����� Ȯ���� �������ϴ�.";
	AddListItem(52, strName, strExplain1, strExplain2, strTip);

	strName =   L"ġŲ";
	strExplain1    = L"ü���� 30 ȸ���մϴ�.";
	strExplain2 = L"";
	strTip = L"����� �������� ����� Ȯ���� �������ϴ�.";
	AddListItem(53, strName, strExplain1, strExplain2, strTip);

	strName =   L"���� Ŭ�ι�";
	strExplain1    = L"ȹ��� ����� 10% �����մϴ�.";
	strExplain2 = L"";
	strTip = L"";
	AddListItem(54, strName, strExplain1, strExplain2, strTip);

	strName =   L"��������";
	strExplain1    = L"���� ������ �Ŀ� ���� ȹ���մϴ�. ������";
	strExplain2    = L"�����Լ��� ȹ���� �� �ֽ��ϴ�.";
	strTip = L"����� �������� �����۰� ��� ȹ�淮�� �������ϴ�.";
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
