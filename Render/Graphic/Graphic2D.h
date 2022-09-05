#pragma once
/////////////////////////////////////////////////////////
//
//	Class : 캡슐화, 정보은닉성, 다형성, 상속성, 추상화
//
//	class내에 선언된 static 함수, static 변수는 C+에서
//	전역으로 사용된것 처럼 이용된다
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