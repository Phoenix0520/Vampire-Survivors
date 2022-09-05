#pragma once

class ListCtrl : public UIObject
{
public:
	ListCtrl();
	~ListCtrl();

public:
	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;

	int   GetItemType(int id);
	bool  GetItemCollect(int id);

	void  UpdateItemType(int id, int type);
	void  UpdateItemCollect(int id, bool val);
	void  UpdateItemCollectEx(int id, bool val);
	void  AddListItem(int id, wstring name, wstring explain1, wstring explain2, wstring tip);
	
private:
	void      SetProperty();
	Vector2   GetFocusItemPosition();
private:
	struct ListItem
	{
		int       id = 0;
		int       type = 0;
		bool      collect =false;
		Vector2   offset;
		Vector2   offsetSize;
		wstring   name;
		wstring   explain1;
		wstring   explain2;
		wstring   tip;
	};
	vector<ListItem*> ListItemDatas;

private:
	class Texture* cDialogue = nullptr;
	class Texture* cOption = nullptr;
	class Texture* texture;
	class Texture* picture = nullptr;
	class Cursor*  cursor = nullptr;
	class Rect*    rect = nullptr;

	int focusItem = 0;

	wstring name = L"";
	wstring explain = L"";
	wstring explain2 = L"";
	wstring tip = L"";
};