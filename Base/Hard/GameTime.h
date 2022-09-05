#pragma once
#define TIMEMANAGER GameTime::GetInstance()

class GameTime
{
public:
	static GameTime* GetInstance()
	{
		static GameTime singletoneInstane; // class ����
		return &singletoneInstane;
	}
	void  Update();
	void  Update(float  lockFPS);                          // �����ð��� Lock�ϰ� Update
	float Delta() { return m_TimeElepased; }              // ����ð�
	float GetRunning() { return m_RunningTime; }           // �� ����ð�
	float GetFPS() { return m_FramePerSecond; }            // Frame�� ����ð�
	int   GetFrame() { return m_FrameCountPerSecond; }     // �ʴ� FrameCount��
	void  WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private: // ����
	float  m_TimeElepased = 0.0f;       // ������ �ð�
	float  m_FPStimeElepased = 0.0f;   // ������ �ð�
	float  m_RunningTime = 0.0f;       // Game������ �� run�ð�
	float  m_FramePerSecond = 0.0f;    // Frame Per Second
	int    m_FrameCount = 0;           // Frame��
	int    m_FrameCountPerSecond = 0;  // �ʴ� frame count
	std::chrono::system_clock::time_point start;

private:
	GameTime();
public:
	~GameTime();
};


