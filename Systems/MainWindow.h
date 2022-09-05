#pragma once
////////////////////////////////////////////////
//  MainWindow�� ���Ѱ��� �����ϱ⶧���� �̱���
//  ���� �����
////////////////////////////////////////////////
#define  MAIN   MainWindow::GetInstance()
class MainWindow
{
public:
	static MainWindow* GetInstance()
	{
		static MainWindow singleTone; // ���⼭ ���ѹ� ����
		return &singleTone;
	}

public:  // Game
	void  Update();      // Ű����,���콺
	void  Render();      //  Rendering

public:
	int   GetWidth() { return m_nWidth; }
	int   GetHeight() { return m_nHeight; }
	HWND  GetWindowHandler() { return m_hWnd; }
	void  SetWindowSize(int width, int height);
	void  CreateBackBuffer();  // win size����
	void  DeleteBackBuffer(); // win size����
	void  OnSize();            // win size����

public:
	void   Init3D();     // DirectX 3D �ʱ�ȭ
	void   CreateInstance(HINSTANCE hInstance, int width, int height);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


private:   // ����
	HINSTANCE  m_hInstance = nullptr;
	HWND       m_hWnd = nullptr;
	int        m_nWidth = 0; // ȭ�� ũ��
	int        m_nHeight = 0;

private:
	MainWindow();
	~MainWindow();
};
