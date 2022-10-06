#pragma once

#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable : 4996)

// Console Define
#ifdef _DEBUG	// 콘솔을 연결해주는 _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")

#endif // _DEBUG

#include "targetver.h"

// Windows 헤더 파일
#include <windows.h>
#include <shellapi.h>
#include <assert.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// STL
#include <iostream>
#include <istream>
#include <string>
#include <vector>
#include <map>
#include <bitset> // 비트를 맞춰준다.
#include <functional>
#include <mutex>
#include <io.h>
#include <chrono>
#include <cmath>
#include <bitset>
#include <mmsystem.h>
#include <random>
#include <ctime>
#pragma comment(lib,"winmm.lib")

// Audio


using namespace std;

// 매크로
#define SAFE_DELETE(p) {if(p) { delete(p); (p) = nullptr;}}
#define SAFE_DELETE_ARRAY(p) {if(p) { delete[](p); p = nullptr; }}
#define SAFE_RELEASE(p) {if(p) { (p)->Release(); (p) = nullptr;}}

_NODISCARD inline wstring f_to_wstring(float _Val)
{	// convert float to wstring
	return (_Floating_to_wstring(L"%.0f", _Val));
}

_NODISCARD inline wstring to_wstring(float _Val, const wchar_t* format)
{	// convert float to wstring
	return (_Floating_to_wstring(format, _Val));
}

// DirectX 환경

// 3D

// header
#include  <d3d11.h>
#include  <D3DX11.h>
#include  <D3DX10.h>
#include  <D3DX10math.h>
//#include  <d3dx11effect.h>
#include  <d3dcompiler.h>

// library
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

// 음원
#include "FMOD/fmod.hpp"
#pragma comment(lib, "./Libraries/FMOD/fmodex_vc.lib")

// IMGUI
//#include "imgui.h"
//#include "imguiDx11.h"
//
//#pragma comment(lib, "./Libraries/ImGui.lib")

#include "imconfig.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#if _WIN64 
#pragma comment(lib,"./Libraries/imGui64.lib")
#else
#pragma comment(lib,"./Libraries/imGui.lib")
#endif

// Typedef
typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;
typedef D3DXCOLOR	Color;
typedef D3DXMATRIX	Matrix;

// 2D

//header
#include <d2d1_2.h>
#include <dwrite.h>

// library
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

// User 정의 디렉토리

// include

// System
#include "MainWindow.h"
#include "GameTime.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Audio.h"
#include "DirectWrite.h"

// Base
#include "Scene.h"
#include "Camera.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "SRVManager.h"
#include "EntityManager.h"
#include "GameObject.h"
#include "UIObject.h"
#include "Collider.h"
#include "Texture.h"

// Animation
#include "Animation.h"
#include "AnimationClip.h"

// Shader
#include "ColorShader.h"
#include "TextureShader.h"

// define
#define MAIN					MainWindow::GetInstance()

#define SCENEMANAGER			SceneManager::GetInstance()
#define TIMEMANAGER				GameTime::GetInstance()
#define DELTA					TIMEMANAGER->Delta()
#define OBJMANAGER				ObjectManager::GetInstance()
#define SRVMANAGER				SRVManager::GetInstance()
#define ENTMANAGER				EntityManager::GetInstance()

#define	CAMERA					Camera::GetInstance()

#define KEYBOARD				Keyboard::GetInstance()
#define DOWN(key)				KEYBOARD->Down(key)
#define UP(key)					KEYBOARD->Up(key)
#define PRESS(key)				KEYBOARD->Press(key)
#define DOWNANY					KEYBOARD->DownAny()
#define UPNANY					KEYBOARD->UpAny()
#define PRESSANY				KEYBOARD->PressAny()

#define PLAYER					(Player*)OBJMANAGER->FindObject("Player")

#define BEGINDRAW				DirectWrite::GetDC()->BeginDraw()
#define ENDDRAW					DirectWrite::GetDC()->EndDraw()

#define VM						CAMERA->GetViewMatrix()
#define PM						CAMERA->GetProjMatrix()
#define CAMPOS					CAMERA->GetPosition()

#define WINWIDTH				1534.0f
#define WINHEIGHT				960.0f

#define ADDSOUND(str, str2, loop)		audio->AddSound(str, str2, loop)
#define PLAYSOUND(str, vol)				audio->Play(str, vol)
#define ISPLAYING(str)					audio->Playing(str)


// Skill & Item Define

#define S_WHIP		0
#define S_MAGIC		1
#define S_SWORD		2
#define S_AXE		3
#define	S_CROSS		4
#define S_BIBLE		5
#define S_FIRE		6
#define S_GARLIC	7

#define I_SPANISH	8
#define I_ARMOR		9
#define I_BLACKHT	10
#define I_REDHT		11
#define I_BOOK		12
#define I_CANDLE	13
#define I_GLOVE		14
#define I_SPELL		15
#define I_RING		16
#define I_WING		17
#define I_CLOVER	18
#define I_CROWN		19

// Key Define
#define ERR_ID		777

// extern
extern ID3D11Device*			Device;
extern ID3D11DeviceContext*		DeviceContext;
extern IDXGISwapChain*			SwapChain;
extern ID3D11RenderTargetView*	rtv;
extern CMouse*					mouse;
extern CAudio*					audio;
extern float					bgmSize;
extern float					sfxSize;
extern bool						visibleStatus;
extern bool						visiblePlayerState;
extern bool						visibleObjID;
extern bool						visibleCollider;
extern bool						visibleDamageCount;
extern int						availableMob;