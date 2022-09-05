#pragma once
/////////////////////////////////////////////////////////
//
//	Class : ĸ��ȭ, �������м�, ������, ��Ӽ�, �߻�ȭ
//
//	class���� ����� static �Լ�, static ������ C+����
//	�������� ���Ȱ� ó�� �̿�ȴ�
//
////////////////////////////////////////////////////////
class Graphic2D
{
public:
	static void Merge();
	static void DrawCircle(HDC hdc, int x, int y, int radius, COLORREF rgb);
	static void DrawLine(HDC hdc, int x1, int y1, int x2, int y2, int width, COLORREF rgb);
	static void DrawRectangle(HDC hdc, int x, int y, int width, int height, COLORREF rgb);
	static void DrawString(HDC hdc, wstring value, int x, int y, int Size, COLORREF rgb);
	static void DrawBitmap(HDC hdc, int Transparent, wstring fileName, int x, int y, COLORREF rgb);
	static void GdiPlusStartUp();
	static void GdiPlusShutDown();
	static void GdiPlusDrawImage(HDC hdc, wstring strFileName, POINT pos, POINT offset, POINT offsetSize);
public:
	Graphic2D();
	~Graphic2D();
};