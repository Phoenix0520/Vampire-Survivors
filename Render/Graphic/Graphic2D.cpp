#include "framework.h"
#include "Graphic2D.h"
#include <ole2.h>  // Object Linking And Emedded : WinOS (OLE,Activex Com Object )
#include <gdiplus.h>
// Gdi Plus
// png������ ����ϱ� ���� 
// 1. include
//    #include  <ole2.h>
//    #include  <gdiplus.h>
// 2. library
//    #pragma comment(lib,"Gdiplus.lib")
// 3. Window Create����  --> Startup
//   static ULONG_PTR gdiplusToken = NULL;
//	 GdiplusStartupInput gdiplusStartInput;
//   GdiplusStartup(&gdiplusToken, &gdiplusStartInput, NULL);
// 4. Window�� ����ɶ�   --> Shutdown
//   GdiplusShutdown(gdiplusToken);
//

#pragma comment(lib,"Gdiplus.lib")
#pragma comment(lib,"msimg32.lib")    // #pragma ������ ���ñ�

using namespace Gdiplus;
static ULONG_PTR gdiplusToken = NULL;

/////////////////////////////////////////////////////////
// ������/ �Ҹ���
/////////////////////////////////////////////////////////
Graphic2D::Graphic2D()
{
}

Graphic2D::~Graphic2D()
{
}
//////////////////////////////////////////////////////////////
//  DrawCircle
//  1.  MyBrush,OldBrush�� �����Ѵ�
//  2.  SelectObject()�� MyBrush�� ��Ͻ�Ų��
//  3.  Drawing
//  4.  �ٽ� ��ȯ�Ѵ�
//  5.  �����Ǿ��� Object�� �����
///////////////////////////////////////////////////////////////
void Graphic2D::DrawCircle(HDC hdc, int cx, int cy, int radius, COLORREF rgb)
{
	//HBRUSH MyBrush, OldBrush;

	//// 1. MyBrush ����
	//MyBrush = CreateSolidBrush(rgb);
	//// 2. MyBrush�� ����ϰ� �Ѵ�
	//OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);
	//// 3. Drawing
	//Ellipse(hdc, cx - radius, cy - radius, cx + radius, cy + radius);
	//// 4. ��ȯ
	//SelectObject(hdc, OldBrush);
	//DeleteObject(MyBrush);

}
//////////////////////////////////////////////////////////////
//  DrawLine
//  1.  MyPen,OldPen�� �����Ѵ�
//  2.  SelectObject()�� Mypen�� ��Ͻ�Ų��
//  3.  Drawing
//  4.  �ٽ� ��ȯ�Ѵ�
//  5.  �����Ǿ��� Object�� �����
///////////////////////////////////////////////////////////////
void Graphic2D::DrawLine(HDC hdc, int x1, int y1, int x2, int y2, int width, COLORREF rgb)
{
	//HPEN  MyPen;      // �װ� ����ϰ��� �ϴ� Pen
	//HPEN  OldPen;     // ������ ���Ǿ��� Pen

	//// 1. �׸����� �ϴ�  Object ���� ( Pen,Brush,Bitmap)
	//MyPen = CreatePen(PS_SOLID, width, rgb);

	//// 2. Object ����ϰ� ������ ���Ǿ��� Pen������ oldPen�� ����
	//OldPen = (HPEN)SelectObject(hdc, MyPen);

	//// 3. Drawing
	//MoveToEx(hdc, x1, y1, NULL);
	//LineTo(hdc, x2, y2);

	//// 4. OldPen���� ���� ��Ų��
	//SelectObject(hdc, OldPen);
	//// 5. �����Ǿ��� MyPen�� Delete�Ѵ�
	//DeleteObject(MyPen);
}
//////////////////////////////////////////////////////////////
//  DrawLine
//  1.  MyBrush,OldBrush�� �����Ѵ�
//  2.  SelectObject()�� MyBrush�� ��Ͻ�Ų��
//  3.  Drawing
//  4.  �ٽ� ��ȯ�Ѵ�
//  5.  �����Ǿ��� Object�� �����
///////////////////////////////////////////////////////////////
void Graphic2D::DrawRectangle(HDC hdc, int x, int y, int width, int height, COLORREF rgb)
{
	//HBRUSH  MyBrush;      // �װ� ����ϰ��� �ϴ� Pen
	//HBRUSH  OldBrush;     // ������ ���Ǿ��� Pen

	//// 1. �׸����� �ϴ�  Object ���� ( Pen,Brush,Bitmap)
	//MyBrush = CreateSolidBrush(rgb);

	//// 2. Object ����ϰ� ������ ���Ǿ��� Pen������ oldPen�� ����
	//OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);

	//// 3. Drawing
	//Rectangle(hdc, x, y, width, height);

	//// 4. OldPen���� ���� ��Ų��
	//SelectObject(hdc, OldBrush);
	//// 5. �����Ǿ��� MyPen�� Delete�Ѵ�
	//DeleteObject(MyBrush);

}
//////////////////////////////////////////////////////////////
// 1. MyFont�� ����
// 2. MyFont�� ����ϰ� �ϰ�, ������ ���Ǿ��� Font�� OldFont�� �����Ѵ�
// 3. Drawing
// 4. OldFont�� ����
// 5. MyFont�� Delete
//////////////////////////////////////////////////////////////
void Graphic2D::DrawString(HDC hdc, wstring value, int x, int y, int Size, COLORREF rgb)
{
	//// 1. FontCreate
	//HFONT MyFont;
	//HFONT OldFont;
	//LOGFONT lf;     // font ���� desc.
	//wstring strFontName = L"Segoe UI";

	//lf.lfHeight = Size;
	//lf.lfWidth = 0;
	//lf.lfEscapement = 0;
	//lf.lfOrientation = 0;     // ���� :0, ����: 1
	//lf.lfWeight = 400;   // ����,����
	//lf.lfItalic = 0;
	//lf.lfUnderline = 0;
	//lf.lfStrikeOut = 0;
	//lf.lfCharSet = HANGUL_CHARSET;
	//lf.lfOutPrecision = 0;
	//lf.lfClipPrecision = 0;
	//lf.lfQuality = 0;
	//lf.lfPitchAndFamily = 0;
	//wcscpy(lf.lfFaceName, strFontName.c_str());

	//MyFont = CreateFontIndirect(&lf);

	//// 2. MyFont�� ����ϰ� �ϰ�, ������ ���Ǿ��� Font�� OldFont�� �����Ѵ�

	//OldFont = (HFONT)SelectObject(hdc, MyFont);

	////3. Drawing
	//SetTextColor(hdc, rgb);
	//SetBkMode(hdc, TRANSPARENT);   // ���ڵ��� ����� ����ó��
	//TextOut(hdc, x, y, value.c_str(), (int)(value.length()));

	////4. OldFont�� ����
	//SelectObject(hdc, OldFont);
	//DeleteObject(MyFont);
}
//////////////////////////////////////////////////////////////
// 1. MemoryDC Create
// 2. MyBitmap�� ����ϰ� �ϰ� ������ ���Ǿ��� Bitmap�� OldBitmap���� 
//    �����Ѵ�
void Graphic2D::DrawBitmap(HDC hdc, int Transparent, wstring fileName, int x, int y, COLORREF rgb)
{
	//// 1. MemoryDC Create
	//HDC MemDC = CreateCompatibleDC(hdc);
	//// 2. Bitmap Create
	//HBITMAP MyBitmap = (HBITMAP)LoadImage(NULL,  // hinstance  --> *.exe
	//	fileName.c_str(), // Bitmap File Name
	//	IMAGE_BITMAP,     // Type IMAGE_BITMAP,IMAGE_ICON,IMAGE_CURSOR
	//	0,  // cx
	//	0,  // cy
	//	LR_LOADFROMFILE | LR_CREATEDIBSECTION);  // DIB( Device Independent Bimap)

	//// 3. Bitmap������ ������  --> �������� Bitmap Size��? 
	//BITMAP  bitmap;
	//GetObject(MyBitmap, sizeof(BITMAP), &bitmap);

	//int Width = bitmap.bmWidth;
	//int Height = bitmap.bmHeight;
	//// 4. MyBitmap�� ��밡���ϰ� �Ѵ�
	//HBITMAP OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);
	//// 5. Rendering
	//if (Transparent != 1)
	//	BitBlt(hdc, x, y, Width, Height, MemDC, 0, 0, SRCCOPY);
	//else
	//{
	//	GdiTransparentBlt(hdc,  // dst hdc
	//		x,    // dst x
	//		y,    // dst y
	//		Width,
	//		Height,
	//		MemDC,  // SRC
	//		0,
	//		0,
	//		Width,
	//		Height,
	//		rgb);
	//}
	//// 6. OldBitmap , MyBitmap
	//SelectObject(MemDC, OldBitmap);
	//DeleteObject(MyBitmap);
	//DeleteDC(MemDC);
}

void Graphic2D::GdiPlusStartUp()
{
	GdiplusStartupInput  gdiplusStartInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartInput, NULL);
}

void Graphic2D::GdiPlusShutDown()
{
	GdiplusShutdown(gdiplusToken);
}

void Graphic2D::GdiPlusDrawImage(HDC hdc, wstring strFileName, POINT pos, POINT offset, POINT offsetSize)
{
	//Gdiplus::Image* image = new Gdiplus::Image(strFileName.c_str());
	//Graphics graphic(hdc);


	////	graphic.DrawImage(image, pos.x, pos.y,1300,1300);

	//RectF dest{ (float)pos.x, (float)pos.y, (float)(offsetSize.x * 1.0f),(float)(offsetSize.y * 1.0f) };

	//graphic.DrawImage(image, dest, (float)offset.x, (float)offset.y, (float)offsetSize.x, (float)offsetSize.y, UnitPixel);


	//// graphic.DrawImage(image, pos.x, pos.y, 0, 0,30,50,UnitPixel);


	//delete image;
}

// ���ϵ��� �� ���Ϸ� ����� ���
void Graphic2D::Merge()
{
	HDC dc = GetDC(MAIN->GetWindowHandler());
	// DC �� handle �� �����ͼ� �׷����� ������ش�.
	Graphics graphic(dc);
	// HDC �� �޾Ƽ� �����Ѵ�.

	Gdiplus::Bitmap* bit = new Gdiplus::Bitmap(100 * 6, 220, &graphic); // Bitmap �̹��� ����
	Graphics* memG = new Graphics(bit);	// Memory DC �� �������ش�. ���⿡ �׸��� �׷��� ������ ������ش�.
	
	Gdiplus::Image* texture1 = new Gdiplus::Image(L"./Images/Background/Chapter (9).png");
	Gdiplus::Image* texture2 = new Gdiplus::Image(L"./Images/Background/UI_Top.png");
	Gdiplus::Image* texture3 = new Gdiplus::Image(L"./Images/Background/UI_Bottom.png");
	
	memG->DrawImage(texture1, 0, 0, texture1->GetWidth(), texture1->GetHeight());
	memG->DrawImage(texture2, 0, 0, texture2->GetWidth(), texture2->GetHeight());
	memG->DrawImage(texture3, 0, 1080 - texture3->GetHeight(), texture3->GetWidth(), texture3->GetHeight());

	texture2->RotateFlip(RotateFlipType::Rotate180FlipY); // ����
	texture3->RotateFlip(RotateFlipType::Rotate180FlipY); // ����

	memG->DrawImage(texture2, 1920 - texture2->GetWidth(), 0, texture2->GetWidth(), texture2->GetHeight());
	memG->DrawImage(texture3, (int)(1920 - texture3->GetWidth()), (int)(1080 - texture3->GetHeight()), texture3->GetWidth(), texture3->GetHeight());

	//int x = 0;
	//Gdiplus::Image* images[30];
	//for (int i = 1; i <= 6; i++)
	//{
	//	wstring str = L"./Images/Trap/Trap (" + to_wstring(i) + L").png";
	//	images[i] = new Gdiplus::Image(str.c_str());
	//	memG->DrawImage(images[i], x, 0, images[i]->GetWidth(), images[i]->GetHeight());
	//	x += 100;
	//	SAFE_DELETE(images[i]);
	//}
	
	wstring writeFileName = L"./Images/RBackGround.png";

	CLSID png;
	CLSIDFromString(L"{557cf406-1a04-11d3-9a73-0000f81ef32e}", &png);
	bit->Save(writeFileName.c_str(), &png);

	delete bit;
	delete memG;

	return;

	//wstring strFolder = L"C:/Bang/�� ����/HellTakerGame_1231/D2D/_Textures";

	//Gdiplus::Image* m_pTexture1 = new Gdiplus::Image(L"C:/Bang/�� ����/HellTakerGame_1231/D2D/_Textures/helltaker/chapter/chapter (1).png");
	//Gdiplus::Image* m_pTexture2 = new Gdiplus::Image(L"C:/Bang/�� ����/HellTakerGame_1231/D2D/_Textures/helltaker/chapter/back1.png");
	//Gdiplus::Image* m_pTexture3 = new Gdiplus::Image(L"C:/Bang/�� ����/HellTakerGame_1231/D2D/_Textures/helltaker/chapter/back2.png");

	//memG->DrawImage(m_pTexture1, 0, 0, m_pTexture1->GetWidth(), m_pTexture1->GetHeight());
	//memG->DrawImage(m_pTexture2, 0, 0, m_pTexture2->GetWidth(), m_pTexture2->GetHeight());
	//memG->DrawImage(m_pTexture3, 0, 1080 - m_pTexture3->GetHeight(),
	//	m_pTexture3->GetWidth(), m_pTexture3->GetHeight());

	//m_pTexture2->RotateFlip(RotateFlipType::Rotate180FlipY); // ����
	//m_pTexture3->RotateFlip(RotateFlipType::Rotate180FlipY); // ����

	//memG->DrawImage(m_pTexture2, 1920 - m_pTexture2->GetWidth(), 0,
	//	m_pTexture2->GetWidth(), m_pTexture2->GetHeight());

	//memG->DrawImage(m_pTexture3, (int)(1920 - m_pTexture3->GetWidth()), (int)(1080 - m_pTexture3->GetHeight()),
	//	m_pTexture3->GetWidth(), m_pTexture3->GetHeight());

	/*
	int x = 0;
	for (int i = 1; i < 13; i++)
	{
		wstring  imgFile;
		wstring  temp;
		imgFile = L"C:/Bang/�� ����/HellTakerGame_1231/D2D/_Textures/helltaker/player/player_Idle/HellTaker (" + to_wstring(i) + L").png";

	//	imgFile = strFolder + temp;
		printf("%ls\n\n", imgFile.c_str());

		Gdiplus::Image* m_pTexture1 = new Gdiplus::Image(imgFile.c_str());
		m_pTexture1->RotateFlip(RotateFlipType::Rotate180FlipY); // ����
		memG->DrawImage(m_pTexture1, x, 0, m_pTexture1->GetWidth(), m_pTexture1->GetHeight());

		x = x + m_pTexture1->GetWidth();
		//delete m_pTexture1;

	}
*/
}