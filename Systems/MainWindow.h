#pragma once
////////////////////////////////////////////////
//  MainWindow는 단한개만 생성하기때문에 싱글톤
//  으로 만든다
////////////////////////////////////////////////
#define  MAIN   MainWindow::GetInstance()
class MainWindow
{
public:
	static MainWindow* GetInstance()
	{
		static MainWindow singleTone; // 여기서 단한번 생성
		return &singleTone;
	}

public:  // Game
	void  Update();      // 키보드,마우스
	void  Render();      //  Rendering

public:
	int   GetWidth() { return m_nWidth; }
	int   GetHeight() { return m_nHeight; }
	HWND  GetWindowHandler() { return m_hWnd; }
	void  SetWindowSize(int width, int height);
	void  CreateBackBuffer();  // win size변경
	void  DeleteBackBuffer(); // win size변경
	void  OnSize();            // win size변경

public:
	void   Init3D();     // DirectX 3D 초기화
	void   CreateInstance(HINSTANCE hInstance, int width, int height);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


private:   // 변수
	HINSTANCE  m_hInstance = nullptr;
	HWND       m_hWnd = nullptr;
	int        m_nWidth = 0; // 화면 크기
	int        m_nHeight = 0;

private:
	MainWindow();
	~MainWindow();
};
