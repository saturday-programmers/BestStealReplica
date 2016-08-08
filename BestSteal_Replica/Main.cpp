#include <windows.h>
#include <mmsystem.h >
#include <d3d9.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "AppCommon.h"
#include "Drawer.h"
#include "Controller.h"
#include "Stage1.h"
#include "Map.h"
#include "Player.h"


namespace BestStealReplica {
static const char* TITLE = "Best Steal Replica";

static IDirect3D9*			 pDirect3D;
static D3DPRESENT_PARAMETERS d3dpp;
static LPDIRECTINPUT8        g_lpDI;
static LPDIRECTINPUTDEVICE8  g_lpDIDevice;

static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
static Controller::Key WINAPI ProcessKBInput();
static void WINAPI DI_Term();
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	using namespace BestStealReplica;

	::_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

	MSG msg;
	HWND hWnd;
	WNDCLASS winc;
	D3DDISPLAYMODE d3ddm;

	//DirectX オブジェクトの生成
	pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);

	//Display Mode の設定
	pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = TRUE;

	//Windows情報の設定
	winc.style = CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc = WndProc;
	winc.cbClsExtra = winc.cbWndExtra = 0;
	winc.hInstance = hInstance;
	winc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winc.hCursor = LoadCursor(NULL, IDC_ARROW);
	winc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	winc.lpszMenuName = NULL;
	winc.lpszClassName = TEXT(TITLE);
	//Windowsの登録
	if (!RegisterClass(&winc)) return 0;

	//Windowsの生成
	int dh = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME) * 2;
	int dw = GetSystemMetrics(SM_CXFRAME) * 2;

	hWnd = CreateWindow(
		TEXT(TITLE),
		TITLE,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0,		// x
		0,		// y
		AppCommon::WINDOW_DEFAULT_WIDTH + dw,		// Width
		AppCommon::WINDOW_DEFAULT_HEIGHT + dh,		// Height
		NULL, NULL, hInstance, NULL
		);
	if (!hWnd) return 0;

	// クライアント領域のサイズを保持
	RECT rc;
	GetClientRect(hWnd, &rc);
	AppCommon::SetWindowHeight(rc.bottom - rc.top);
	AppCommon::SetWindowWidth(rc.right - rc.left);

	// DirectInputオブジェクト作成
	HRESULT hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_lpDI, NULL);
	if FAILED(hr) {
		// DirectInput not available; take appropriate action 
		return false;
	}

	// DirectInputキーボードデバイスの作成
	hr = g_lpDI->CreateDevice(GUID_SysKeyboard, &g_lpDIDevice, NULL);
	if FAILED(hr) {
		DI_Term();
		return FALSE;
	}

	// キーボードデータフォーマットの設定
	hr = g_lpDIDevice->SetDataFormat(&c_dfDIKeyboard);
	if FAILED(hr) {
		DI_Term();
		return FALSE;
	}

	// キーボードの動作の設定 
	hr = g_lpDIDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if FAILED(hr) {
		DI_Term();
		return FALSE;
	}

	Drawer drawer(hWnd, pDirect3D, &d3dpp);
	IStage* pStage1 = new Stage1();
	Controller controller(&drawer);
	controller.LoadStage(pStage1);

	DWORD SyncOld = timeGetTime();
	DWORD SyncNow;

	timeBeginPeriod(1);

	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT) {
		Sleep(1);
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			SyncNow = timeGetTime();
			if (SyncNow - SyncOld >= 1000 / 60) {
				Controller::Key key = ProcessKBInput();
				controller.Control(key);
				controller.Draw();

				SyncOld = SyncNow;
			}
		}
	}

	timeEndPeriod(1);

	// 終了処理
	DI_Term();
	drawer.Release();
	controller.Release();
	delete pStage1;

	return (int)msg.wParam;
}


namespace BestStealReplica {

//-------------------------------------------------------------
//
//	メッセージ処理
//
//-------------------------------------------------------------
static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hWnd, msg, wp, lp);
}

static Controller::Key WINAPI ProcessKBInput() {
	#define KEYDOWN(name, key) (name[key] & 0x80) 

	Controller::Key ret;

	// キーボードへのアクセスの獲得
	if (g_lpDIDevice) {
		g_lpDIDevice->Acquire();
	}

	char     buffer[256];
	HRESULT  hr;
	hr = g_lpDIDevice->GetDeviceState(sizeof(buffer), (LPVOID)&buffer);
	if FAILED(hr) {
		// If it failed, the device has probably been lost. 
		// Check for (hr == DIERR_INPUTLOST) 
		// and attempt to reacquire it here. 
		ret.keyType = Controller::Key::KeyType::NONE;
		return ret;
	}

	if (KEYDOWN(buffer, DIK_Z)) {
		ret.keyType = Controller::Key::KeyType::STEAL;
	} else if (KEYDOWN(buffer, DIK_RIGHT)) {
		ret.keyType = Controller::Key::KeyType::RIGHT;
	} else if (KEYDOWN(buffer, DIK_LEFT)) {
		ret.keyType = Controller::Key::KeyType::LEFT;
	} else if (KEYDOWN(buffer, DIK_UP)) {
		ret.keyType = Controller::Key::KeyType::UP;
	} else if (KEYDOWN(buffer, DIK_DOWN)) {
		ret.keyType = Controller::Key::KeyType::DOWN;
	} else {
		return ret;
	}

	if (KEYDOWN(buffer, DIK_LSHIFT) || KEYDOWN(buffer, DIK_RSHIFT)) {
		ret.isWalking = true;
	}

	return ret;
}


static void WINAPI DI_Term(void) {
	if (g_lpDI) {
		if (g_lpDIDevice) {
			// Always unacquire device before calling Release(). 
			g_lpDIDevice->Unacquire();
			g_lpDIDevice->Release();
			g_lpDIDevice = NULL;
		}
		g_lpDI->Release();
		g_lpDI = NULL;
	}
}

}