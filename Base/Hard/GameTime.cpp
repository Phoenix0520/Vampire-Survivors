#include "framework.h"
#include "GameTime.h"

GameTime::GameTime()
{
	start = std::chrono::system_clock::now();  // 현재시간
}

GameTime::~GameTime()
{
}

void GameTime::Update()
{
	std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;

	m_TimeElepased = (float)sec.count();
	start = std::chrono::system_clock::now();
	m_FrameCount++;
	m_RunningTime = m_RunningTime + m_TimeElepased;

	m_FPStimeElepased = m_FPStimeElepased + m_TimeElepased;

	if (m_FPStimeElepased > 1.0f)
	{
		m_FramePerSecond = m_FPStimeElepased / m_FrameCount;
		m_FrameCountPerSecond = m_FrameCount;
		m_FPStimeElepased = 0.0f;
		m_FrameCount = 0;
	}
}

void GameTime::Update(float lockFPS)
{
	std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;

	m_TimeElepased = (float)sec.count();
	while (m_TimeElepased < 1.0f / lockFPS)
	{
		sec = std::chrono::system_clock::now() - start;
		m_TimeElepased = (float)sec.count();

	}

	start = std::chrono::system_clock::now();
	m_FrameCount++;
	m_RunningTime = m_RunningTime + m_TimeElepased;

	m_FPStimeElepased = m_FPStimeElepased + m_TimeElepased;

	if (m_FPStimeElepased > 1.0f)
	{
		m_FramePerSecond = m_FPStimeElepased / m_FrameCount;
		m_FrameCountPerSecond = m_FrameCount;
		m_FPStimeElepased = 0.0f;
		m_FrameCount = 0;
	}
}

void GameTime::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_TIMER:
		MessageBox(hWnd, L"Timer", L"Title", MB_OK);
		break;


	}
}


