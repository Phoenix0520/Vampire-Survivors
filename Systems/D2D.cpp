 #include "framework.h"
#include "D2D.h"
#include <fstream>

CMouse* mouse = nullptr;
CAudio* audio = nullptr;

void LoadFont();
////////////////////////////////////////////////////
// 프로그램의 진입점
////////////////////////////////////////////////////
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
// Window 생성, Direct X11 세팅
{
	srand((GetTickCount()));

	audio = new CAudio();

	MAIN->CreateInstance(hInstance, (int)WINWIDTH, (int)WINHEIGHT);
	// width, height 를 각각 0 으로 해주면 전체화면이 되도록한다.

	mouse = new CMouse(MAIN->GetWindowHandler());

	// ImGUI Setting
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplWin32_Init(MAIN->GetWindowHandler());
	ImGui_ImplDX11_Init(Device, DeviceContext);
	ImGui::StyleColorsDark();
	LoadFont();

	// 2D Create
	DirectWrite::Create();

	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			TIMEMANAGER->Update(60.0f);
			mouse->Update();
			audio->Update();
			MAIN->Update();
			//ImGui::Update();
			//ImGui_ImplDX11_NewFrame();
			//ImGui_ImplWin32_NewFrame();
			//ImGui::NewFrame();
			MAIN->Render();
		}
	}

	SAFE_DELETE(audio);
	SAFE_DELETE(mouse);
	DirectWrite::Delete();

	// IMGUI DELETE

	//ImGui::
	//ImGui::Delete();
	
	return (int)msg.wParam;
}

void  LoadFont()
{
	ImGuiIO& io = ImGui::GetIO();
	ImFont* font = nullptr;
	string fontfile = "./TTF/malgunbd.ttf";

	ifstream ifile;
	ifile.open(fontfile);

	if (ifile)
	{
		io.Fonts->AddFontFromFileTTF(fontfile.c_str(),
			16.0f, NULL, io.Fonts->GetGlyphRangesKorean());
	}

}