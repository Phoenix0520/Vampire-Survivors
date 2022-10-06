#include "framework.h"
#include "MainWindow.h"
#include "Resource.h"
#include "Player.h"

// 생성자 소멸자
ID3D11Device*			Device = nullptr; // CPU 영역
ID3D11DeviceContext*	DeviceContext = nullptr; // GPU 영역
IDXGISwapChain*			SwapChain = nullptr; // 더블 버퍼링을 위한 것,
ID3D11RenderTargetView* rtv = nullptr; // 실제 그려질 영역
float					bgmSize = 0.0f;
float					sfxSize = 0.0f;
bool					visibleStatus = false;
bool					visiblePlayerState = false;
bool					visibleCollider = false;
bool					visibleObjID = false;
bool					visibleDamageCount = false;
int						availableMob = 0;


//////////////////////////////////////////////
// 생성자 / 소멸자
//////////////////////////////////////////////
MainWindow::MainWindow()
{
}

MainWindow::~MainWindow()
{
	SAFE_RELEASE(Device);
	SAFE_RELEASE(DeviceContext);
	SAFE_RELEASE(SwapChain);
	SAFE_RELEASE(rtv);
}


void MainWindow::Update()
{
	SCENEMANAGER->Update();
}

//////////////////////////////////////////////
// IA->VS->PS->RasterStage->OM
//////////////////////////////////////////////
void MainWindow::Render()
{
	D3D11_VIEWPORT viewport;
	{
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.TopLeftX = 0;            // 시작점   
		viewport.TopLeftY = 0;
		viewport.Width = (float)m_nWidth; // 종단점
		viewport.Height = (float)m_nHeight;
	}

	DeviceContext->OMSetRenderTargets(1, &rtv, NULL); // OM Statge
	DeviceContext->RSSetViewports(1, &viewport);      // RS Stage

	// Crear Color
	Color  bgColor = Color(0.0f, 0.0f, 0.0f, 1.0f);
	DeviceContext->ClearRenderTargetView(rtv, (float*)bgColor);

	//  신을 불러서 Rendering
	//  ---------------------------------------

	SCENEMANAGER->Render();
	//ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	SwapChain->Present(0, 0);
}
void MainWindow::SetWindowSize(int width, int height)
{
	MoveWindow(m_hWnd, 50, 50, width, height, TRUE);

	RECT rect;

	GetClientRect(m_hWnd, &rect);
	m_nWidth = rect.right - rect.left;
	m_nHeight = rect.bottom - rect.top;
}
/////////////////////////////////////////////////////////////////
//   2. SwapChain으로 Backbuffer를 얻어와  CreateRenderTargetView를 생성한다
////////////////////////////////////////////////////////////////
void MainWindow::CreateBackBuffer()
{
	// Get Backbuffer
	ID3D11Texture2D* BackBuffer;
	// com interface --> typelib ( Automation기법  --> Excel 
	HRESULT hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
	assert(SUCCEEDED(hr));

	// Create RenderTargetView
	hr = Device->CreateRenderTargetView(BackBuffer, NULL, &rtv);
	assert(SUCCEEDED(hr));
	SAFE_RELEASE(BackBuffer);


}

void MainWindow::DeleteBackBuffer()
{
	SAFE_RELEASE(rtv);
}

void MainWindow::OnSize()
{
	return;

	RECT rect;
	GetClientRect(this->GetWindowHandler(), &rect);

	m_nWidth = rect.right - rect.left;
	m_nHeight = rect.bottom - rect.top;

	if (Device != NULL)
	{
		//	ImGui::Invalidate();
		DeleteBackBuffer();
		//	DirectWrite::DeleteBackBuffer();

		HRESULT hr = SwapChain->ResizeBuffers(0, m_nWidth, m_nHeight, DXGI_FORMAT_UNKNOWN, 0);
		assert(SUCCEEDED(hr));
		DirectWrite::CreateBackBuffer();
		//	CreateBackBuffer();
			//	ImGui::Validate();
	}

}

////////////////////////////////////////////
// DirectX 초기화
// Device,DeviceContext,SwapChain을 생성
// com intetface   : description이 필요하다
////////////////////////////////////////////
void MainWindow::Init3D()
{

	//1 .  Device,DeviceContext,SwapChain을 생성
	DXGI_MODE_DESC BufferDesc;
	{
		ZeroMemory(&BufferDesc, sizeof(DXGI_MODE_DESC));
		BufferDesc.Width = m_nWidth;  // 화면의 넓이
		BufferDesc.Height = m_nHeight; // 화면의 높이
		BufferDesc.RefreshRate.Numerator = 60; // 분자
		BufferDesc.RefreshRate.Denominator = 1; // 분모 (1초에 60번)
		BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // 정규화
		BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	}

	DXGI_SWAP_CHAIN_DESC swapDesc;  // 변수
	{
		ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		swapDesc.BufferDesc = BufferDesc;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.BufferCount = 1;         // 2D,3D
		swapDesc.SampleDesc.Count = 1;    // 기본형
		swapDesc.SampleDesc.Quality = 0;  // 기본형
		swapDesc.OutputWindow = m_hWnd;   // 현재 윈도우의 핸들
		swapDesc.Windowed = TRUE;
		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // swap이후에 버려진다
	}

	vector<D3D_FEATURE_LEVEL> feature_level =
	{
	   D3D_FEATURE_LEVEL_11_1,
	   D3D_FEATURE_LEVEL_11_0,
	   D3D_FEATURE_LEVEL_10_0,
	   D3D_FEATURE_LEVEL_9_3,
	   D3D_FEATURE_LEVEL_9_2,
	  D3D_FEATURE_LEVEL_9_1,
	};


	D3D_FEATURE_LEVEL pFeatureLevels;


	// SwapChain, Device, DeviceContext를 생성

	HRESULT hr = D3D11CreateDeviceAndSwapChain
	(
		NULL,					  //pAdapter --> 비데오카드를 지정하지 않을때는 기본
		D3D_DRIVER_TYPE_HARDWARE, // 그래픽카드가 었는 경우(D3D_DRIVER_TYPE_SOFTWARE) 
		NULL, // Software : thidy party에서 제공되는 Lib
		D3D11_CREATE_DEVICE_BGRA_SUPPORT, //  2D+3D혼재 사용 FLAG
		feature_level.data(), // 지원가능한 우선순위를 설정한 데이터
		feature_level.size(), // 위에 선언된 데이터의 크기
		D3D11_SDK_VERSION,
		&swapDesc,
		&SwapChain,    //**ppSwapChain   : output SwapChain
		&Device,       //**ppDevice      : output Device
		&pFeatureLevels,  // 우선순위 배정한것에 FeatureLevel
		&DeviceContext
	);

	assert(SUCCEEDED(hr));

	//2 . Buffer생성 
	//    SwapChain으로 부터 Backbuffer를 얻어와 RTV를 생성

	CreateBackBuffer();

}

void MainWindow::CreateInstance(HINSTANCE hInstance, int width, int height)
{
	{	// 윈도우 클래스 등록
		WNDCLASSEXW wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D2D));
		// MAKEINTRESOURCE 는 리소스에서 뭔가를 가져오겠다는것.
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);

		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		if (width == 0 && height == 0)
			wcex.lpszMenuName = NULL;
		else
			wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_D2D);
		// MenuName 에 NULL 을 넣으면 메뉴이름창이 나오지않는다.

		wcex.lpszClassName = L"SGA_ILSAN";
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		RegisterClassExW(&wcex);
	}

	m_hInstance = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	if (width == 0 && height == 0)
	{
		m_nWidth = GetSystemMetrics(SM_CXSCREEN);
		m_nHeight = GetSystemMetrics(SM_CYSCREEN);

		m_hWnd = CreateWindowW // Window 로 만들어줄것들을 만들어준다.
		(
			L"SGA_ILSAN",
			L"Vampire Survivors",
			WS_POPUP,
			0,
			0,
			m_nWidth,
			m_nHeight,
			nullptr,
			nullptr,
			hInstance,
			nullptr
		);

		width = m_nWidth;
		height = m_nHeight;
	}
	else
	{
		m_hWnd = CreateWindowW // Window 로 만들어줄것들을 만들어준다.
		(
			L"SGA_ILSAN",		    // 클래스의 이름
			L"Vampire Survivors",	// 제목의 이름
			dwStyle,				// 디스플레이 스타일
			0,						// x 의 위치 
			0,						// y 의 위치
			width,					// 너비
			height,					// 높이
			nullptr,				// 다이얼로그를 사용할때는 Window 의 부모객체가 필요한데 일단은 없으니까 nullptr
			nullptr,				// 윈도우에 메뉴 바, 툴 바 가 필요할 경우 넣는다. 마찬가지로 nullptr
			hInstance,				// OS가 처음에 할당해준 프로그램의 Instance
			nullptr					// 파라미터 설정 일단 안하니까 nullptr
		);

		// 실제 화면 비율을 맞출경우
		RECT rect;
		GetClientRect(m_hWnd, &rect);
		// 현재 화면비를 구한다.

		int gapX = width - (rect.right - rect.left);
		int gapY = height - (rect.bottom - rect.top);
		// 설정한 비율에서 현재 화면비를 뺸다.

		int w = GetSystemMetrics(SM_CXSCREEN);
		int h = GetSystemMetrics(SM_CYSCREEN);

		HWND taskbar = FindWindow(_T("Shell_TrayWnd"), NULL);//작업표시줄 핸들러 가져오기

		if (taskbar)
			::GetWindowRect(taskbar, &rect);

		int gap = rect.top - rect.bottom;

		MoveWindow(m_hWnd, (w - width) / 2 - 8, (h - height) / 2 + gap, width + gapX, height + gapY, TRUE);
		// MoveWindow 로 차이값을 더해준다.

		GetClientRect(m_hWnd, &rect);

		m_nWidth = width;
		m_nHeight = height;
		// 내 윈도우 창 크기를 저장해놓는다.
	}

	Init3D();
	// Init3D 를 통해 시작
	ShowWindow(m_hWnd, SW_SHOWDEFAULT); // nCmdShow SW_ 에 여러가지 방식이있다. 소프트웨어가 시작할때에 화면을 선택한다.
	// 윈도우를 보여주는 역할
	UpdateWindow(m_hWnd);

	SCENEMANAGER->GetInstance();
	// 마지막에 씬매니져 를 가져와준다.
}


LRESULT MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);

	if (mouse != nullptr)
	{
		mouse->WndProc(message, wParam, lParam);
	}

	// 메세지 처리함수
	switch (message)
	{
	case WM_KEYDOWN:
	{
		if (visibleStatus)
		{
			cout << endl;

			switch ((char)wParam)
			{
			case ' ':
				cout << "KeyDown : space" << endl;
				break;
			case '	':
				cout << "KeyDown : tab" << endl;
				break;
			case VK_RETURN:
				cout << "KeyDown : enter" << endl;
				break;
			case '&':
				cout << "KeyDown : ↑" << endl;
				break;
			case '\'':
				cout << "KeyDown : →" << endl;
				break;
			case '(':
				cout << "KeyDown : ↓" << endl;
				break;
			case '%':
				cout << "KeyDown : ←" << endl;
				break;
			default:
				cout << "KeyDown : " << (char)wParam << endl;
				break;
				
				cout << endl;
			}
		}

		Player* player = PLAYER;

		switch (wParam)
		{
		case VK_TAB:
			if (visibleStatus)
				visibleStatus = false;
			else
				visibleStatus = true;
			break;
		case VK_F1:
			if (visibleCollider)
				visibleCollider = false;
			else
				visibleCollider = true;
			break;
		case VK_F2:
			ENTMANAGER->ToggleVisible();
			break;
		case VK_F3:
			if (visiblePlayerState)
				visiblePlayerState = false;
			else
				visiblePlayerState = true;
			break;
		case VK_F4:
			if (visibleObjID)
				visibleObjID = false;
			else
				visibleObjID = true;
			break;
		}
		break;
	}
	case WM_KEYUP:
	{
		KEYBOARD->WindowCallbackKeyUp((int)wParam);
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}