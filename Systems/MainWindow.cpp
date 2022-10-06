#include "framework.h"
#include "MainWindow.h"
#include "Resource.h"
#include "Player.h"

// ������ �Ҹ���
ID3D11Device*			Device = nullptr; // CPU ����
ID3D11DeviceContext*	DeviceContext = nullptr; // GPU ����
IDXGISwapChain*			SwapChain = nullptr; // ���� ���۸��� ���� ��,
ID3D11RenderTargetView* rtv = nullptr; // ���� �׷��� ����
float					bgmSize = 0.0f;
float					sfxSize = 0.0f;
bool					visibleStatus = false;
bool					visiblePlayerState = false;
bool					visibleCollider = false;
bool					visibleObjID = false;
bool					visibleDamageCount = false;
int						availableMob = 0;


//////////////////////////////////////////////
// ������ / �Ҹ���
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
		viewport.TopLeftX = 0;            // ������   
		viewport.TopLeftY = 0;
		viewport.Width = (float)m_nWidth; // ������
		viewport.Height = (float)m_nHeight;
	}

	DeviceContext->OMSetRenderTargets(1, &rtv, NULL); // OM Statge
	DeviceContext->RSSetViewports(1, &viewport);      // RS Stage

	// Crear Color
	Color  bgColor = Color(0.0f, 0.0f, 0.0f, 1.0f);
	DeviceContext->ClearRenderTargetView(rtv, (float*)bgColor);

	//  ���� �ҷ��� Rendering
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
//   2. SwapChain���� Backbuffer�� ����  CreateRenderTargetView�� �����Ѵ�
////////////////////////////////////////////////////////////////
void MainWindow::CreateBackBuffer()
{
	// Get Backbuffer
	ID3D11Texture2D* BackBuffer;
	// com interface --> typelib ( Automation���  --> Excel 
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
// DirectX �ʱ�ȭ
// Device,DeviceContext,SwapChain�� ����
// com intetface   : description�� �ʿ��ϴ�
////////////////////////////////////////////
void MainWindow::Init3D()
{

	//1 .  Device,DeviceContext,SwapChain�� ����
	DXGI_MODE_DESC BufferDesc;
	{
		ZeroMemory(&BufferDesc, sizeof(DXGI_MODE_DESC));
		BufferDesc.Width = m_nWidth;  // ȭ���� ����
		BufferDesc.Height = m_nHeight; // ȭ���� ����
		BufferDesc.RefreshRate.Numerator = 60; // ����
		BufferDesc.RefreshRate.Denominator = 1; // �и� (1�ʿ� 60��)
		BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // ����ȭ
		BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	}

	DXGI_SWAP_CHAIN_DESC swapDesc;  // ����
	{
		ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		swapDesc.BufferDesc = BufferDesc;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.BufferCount = 1;         // 2D,3D
		swapDesc.SampleDesc.Count = 1;    // �⺻��
		swapDesc.SampleDesc.Quality = 0;  // �⺻��
		swapDesc.OutputWindow = m_hWnd;   // ���� �������� �ڵ�
		swapDesc.Windowed = TRUE;
		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // swap���Ŀ� ��������
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


	// SwapChain, Device, DeviceContext�� ����

	HRESULT hr = D3D11CreateDeviceAndSwapChain
	(
		NULL,					  //pAdapter --> �񵥿�ī�带 �������� �������� �⺻
		D3D_DRIVER_TYPE_HARDWARE, // �׷���ī�尡 ���� ���(D3D_DRIVER_TYPE_SOFTWARE) 
		NULL, // Software : thidy party���� �����Ǵ� Lib
		D3D11_CREATE_DEVICE_BGRA_SUPPORT, //  2D+3Dȥ�� ��� FLAG
		feature_level.data(), // ���������� �켱������ ������ ������
		feature_level.size(), // ���� ����� �������� ũ��
		D3D11_SDK_VERSION,
		&swapDesc,
		&SwapChain,    //**ppSwapChain   : output SwapChain
		&Device,       //**ppDevice      : output Device
		&pFeatureLevels,  // �켱���� �����ѰͿ� FeatureLevel
		&DeviceContext
	);

	assert(SUCCEEDED(hr));

	//2 . Buffer���� 
	//    SwapChain���� ���� Backbuffer�� ���� RTV�� ����

	CreateBackBuffer();

}

void MainWindow::CreateInstance(HINSTANCE hInstance, int width, int height)
{
	{	// ������ Ŭ���� ���
		WNDCLASSEXW wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D2D));
		// MAKEINTRESOURCE �� ���ҽ����� ������ �������ڴٴ°�.
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);

		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		if (width == 0 && height == 0)
			wcex.lpszMenuName = NULL;
		else
			wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_D2D);
		// MenuName �� NULL �� ������ �޴��̸�â�� �������ʴ´�.

		wcex.lpszClassName = L"SGA_ILSAN";
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		RegisterClassExW(&wcex);
	}

	m_hInstance = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	if (width == 0 && height == 0)
	{
		m_nWidth = GetSystemMetrics(SM_CXSCREEN);
		m_nHeight = GetSystemMetrics(SM_CYSCREEN);

		m_hWnd = CreateWindowW // Window �� ������ٰ͵��� ������ش�.
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
		m_hWnd = CreateWindowW // Window �� ������ٰ͵��� ������ش�.
		(
			L"SGA_ILSAN",		    // Ŭ������ �̸�
			L"Vampire Survivors",	// ������ �̸�
			dwStyle,				// ���÷��� ��Ÿ��
			0,						// x �� ��ġ 
			0,						// y �� ��ġ
			width,					// �ʺ�
			height,					// ����
			nullptr,				// ���̾�α׸� ����Ҷ��� Window �� �θ�ü�� �ʿ��ѵ� �ϴ��� �����ϱ� nullptr
			nullptr,				// �����쿡 �޴� ��, �� �� �� �ʿ��� ��� �ִ´�. ���������� nullptr
			hInstance,				// OS�� ó���� �Ҵ����� ���α׷��� Instance
			nullptr					// �Ķ���� ���� �ϴ� ���ϴϱ� nullptr
		);

		// ���� ȭ�� ������ ������
		RECT rect;
		GetClientRect(m_hWnd, &rect);
		// ���� ȭ��� ���Ѵ�.

		int gapX = width - (rect.right - rect.left);
		int gapY = height - (rect.bottom - rect.top);
		// ������ �������� ���� ȭ��� �A��.

		int w = GetSystemMetrics(SM_CXSCREEN);
		int h = GetSystemMetrics(SM_CYSCREEN);

		HWND taskbar = FindWindow(_T("Shell_TrayWnd"), NULL);//�۾�ǥ���� �ڵ鷯 ��������

		if (taskbar)
			::GetWindowRect(taskbar, &rect);

		int gap = rect.top - rect.bottom;

		MoveWindow(m_hWnd, (w - width) / 2 - 8, (h - height) / 2 + gap, width + gapX, height + gapY, TRUE);
		// MoveWindow �� ���̰��� �����ش�.

		GetClientRect(m_hWnd, &rect);

		m_nWidth = width;
		m_nHeight = height;
		// �� ������ â ũ�⸦ �����س��´�.
	}

	Init3D();
	// Init3D �� ���� ����
	ShowWindow(m_hWnd, SW_SHOWDEFAULT); // nCmdShow SW_ �� �������� ������ִ�. ����Ʈ��� �����Ҷ��� ȭ���� �����Ѵ�.
	// �����츦 �����ִ� ����
	UpdateWindow(m_hWnd);

	SCENEMANAGER->GetInstance();
	// �������� ���Ŵ��� �� �������ش�.
}


LRESULT MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);

	if (mouse != nullptr)
	{
		mouse->WndProc(message, wParam, lParam);
	}

	// �޼��� ó���Լ�
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
				cout << "KeyDown : ��" << endl;
				break;
			case '\'':
				cout << "KeyDown : ��" << endl;
				break;
			case '(':
				cout << "KeyDown : ��" << endl;
				break;
			case '%':
				cout << "KeyDown : ��" << endl;
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