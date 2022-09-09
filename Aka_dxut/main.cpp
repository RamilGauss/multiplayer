#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "SaveOnHDD.h"
#include <winbase.h>
#include <time.h>
#include "MyGUI_Types.h"
#include <winnt.rh>
#include "GUI_DescWindow.h"
#include "BaseGUI.h"
#include "ClientMain.h"
#include "GameRoomPrepare.h"
#include "WaitForm.h"
#include "MakerGUI.h"
#include "IGUI.h"

TSaveOnHDD mLog;

using namespace MyGUI;

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dxof.lib")
#pragma comment(lib,"dxguid.lib")

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//--------------------------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D;         
LPDIRECT3DDEVICE9       g_pD3DDevice;   
D3DPRESENT_PARAMETERS   g_D3DPP;

HWND g_hWnd;
bool g_windowed = true;

IGUI* pGUI;
TClientMain      g_ClientMain;
TGameRoomPrepare g_GameRoomPrepare;
TWaitForm        g_WaitForm;
//--------------------------------------------------------------------------------------------
bool flgNeedInitDirectX = true;
//--------------------------------------------------------------------------------------------
void DirectXCreate(HWND hWnd, int width, int height);
void DirectXRender();
void DirectXDestroy();
void DirectXMatrix();
void DirectXRenderState();
void InitMyGUI();
void DestroyMyGUI();
//--------------------------------------------------------------------------------------------
// имя класса окна
#define WND_CLASS_NAME L"MyGUI_Demo_window"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  TMakerGUI makerGUI;
  pGUI = makerGUI.New();

  int w = 900,
      h = 500;

  WNDPROC baseWndProc = WndProc;

  WNDCLASS wndClass;
  wndClass.style = CS_DBLCLKS;
  wndClass.lpfnWndProc = baseWndProc;
  wndClass.cbClsExtra = 0;
  wndClass.cbWndExtra = 0;
  wndClass.hInstance = GetModuleHandle(NULL);
  wndClass.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(1001));
  wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );
  wndClass.hbrBackground = ( HBRUSH )GetStockObject( BLACK_BRUSH );
  wndClass.lpszMenuName = NULL;
  wndClass.lpszClassName = WND_CLASS_NAME;

  RegisterClass( &wndClass );

  g_hWnd = CreateWindow(
    wndClass.lpszClassName, TEXT("Direct3D9 Render Window"), WS_POPUP, 
    0, 0, w, h, GetDesktopWindow(), NULL, wndClass.hInstance, NULL);

  ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);
	DirectXCreate(g_hWnd,w,h);
  flgNeedInitDirectX = false;
	InitMyGUI();
	MSG msg;
	while(TRUE)
	{
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
      TranslateMessage(&msg);
      DispatchMessage(&msg);
		}
    if(msg.message==WM_QUIT) 
      break;  

    DirectXRender();

    if (GetActiveWindow() != g_hWnd)
      ::Sleep(50);
	}
	DestroyMyGUI();
	DirectXDestroy();
  mLog.ReOpen("render_Timer.xls");
	return (int)msg.wParam;
}
//------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
    case WM_PAINT:
      if(!flgNeedInitDirectX)
        DirectXRender();
      break;
    case WM_SIZE:
      if (wParam != SIZE_MINIMIZED)
        pGUI->Resize();
      break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
  }
  return DefWindowProc(hWnd, msg, wParam, lParam);
}
//------------------------------------------------------------------------------
void DirectXCreate(HWND hWnd, int width, int height)
{
	D3DDISPLAYMODE d3ddm;

	if((g_pD3D = ::Direct3DCreate9(D3D_SDK_VERSION)) == 0) 
		return;
	if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) 
		return;
	ZeroMemory(&g_D3DPP, sizeof(D3DPRESENT_PARAMETERS));
	g_D3DPP.BackBufferCount        = 1;
	g_D3DPP.Windowed               = g_windowed; 
	g_D3DPP.BackBufferFormat       = d3ddm.Format; 
  g_D3DPP.BackBufferWidth        = width;
  g_D3DPP.BackBufferHeight       = height;
  g_D3DPP.SwapEffect             = D3DSWAPEFFECT_FLIP;// D3DSWAPEFFECT_DISCARD; 
	g_D3DPP.EnableAutoDepthStencil = TRUE; 
	g_D3DPP.AutoDepthStencilFormat = D3DFMT_D16;
  g_D3DPP.BackBufferFormat       = d3ddm.Format;
  g_D3DPP.hDeviceWindow          = hWnd;

	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_D3DPP, &g_pD3DDevice))) 
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &g_D3DPP, &g_pD3DDevice))) 
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &g_D3DPP, &g_pD3DDevice))) 
		return;
}
//------------------------------------------------------------------------------
uint32 start = GetTickCount();
void DirectXRender()
{
  uint32 delta = GetTickCount() - start;
  start = GetTickCount();
  mLog.WriteF("%u\n",delta);

	if(g_pD3DDevice->BeginScene()==S_OK)
  {
    g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

      // RenderGUI
      pGUI->Render();  

    g_pD3DDevice->EndScene();
  }
  
  if(g_pD3DDevice->Present(0,0,0,0) == D3DERR_DEVICELOST)
    pGUI->LostDevice();
}
//------------------------------------------------------------------------------
void DirectXDestroy()
{
  SAFE_RELEASE(g_pD3DDevice);
	SAFE_RELEASE(g_pD3D);
}
//------------------------------------------------------------------------------
void InitMyGUI()
{
  //------------------------CORE----------------------
  // добавить окно, созданное движком в GUI
  TGUIDescWindow descWindow;
  descWindow.hwnd       = g_hWnd;
  descWindow.pD3D       = g_pD3D;
  descWindow.pD3DDevice = g_pD3DDevice;
  descWindow.pD3DPP     = &g_D3DPP;
  descWindow.windowed   = g_windowed;
  pGUI->AddWindow(&descWindow);

  //---------------------------------------------------
  pGUI->AddResourceLocation("/Demos/Demo_Themes");
  pGUI->AddResourceLocation("/Common/Demos");
  pGUI->AddResourceLocation("/Common/Themes");
  MyGUI::ResourceManager::getInstance().load("MyGUI_BlackBlueTheme.xml");// загрузка скина
  //------------------------CORE----------------------

  //------------------------INIT_DEV_TOOL-----------------------
  // создание форм
  //g_GameRoomPrepare = new TGameRoomPrepare;
  //g_ClientMain = new TClientMain;
  //g_WaitForm = new TWaitForm;
  // добавить в менеджер для общего доступа
  pGUI->Add(std::string("g_ClientMainForm"),&g_ClientMain);
  pGUI->Add(std::string("g_GameRoomPrepare"),&g_GameRoomPrepare);
  pGUI->Add(std::string("g_WaitForm"),&g_WaitForm);
  // показать форму
  g_ClientMain.Show();
  //g_GameRoomPrepare->Show();
  //g_WaitForm->Show();
  // подстроиться
  pGUI->Resize();
  //------------------------INIT_DEV_TOOL-----------------------
}
//------------------------------------------------------------------------------
void DestroyMyGUI()
{
  g_ClientMain.Shutdown();
  //delete g_ClientMain;
  //g_ClientMain = nullptr;

  g_GameRoomPrepare.Shutdown();
  //delete g_GameRoomPrepare;
  //g_GameRoomPrepare = nullptr;

  g_WaitForm.Shutdown();
  //delete g_WaitForm;
  //g_WaitForm = nullptr;

  pGUI->Clear();

  pGUI->Destroy();
}
//------------------------------------------------------------------------------
void FullScreen(HWND hWnd, int width, int height, bool fullScreen)
{
  // стиль окна
  HWND hwndAfter = 0;
  unsigned long style = 0;
  unsigned long style_ex = 0;

  RECT rc = { 0, 0, width, height };

  if (fullScreen)
  {
    style = WS_POPUP | WS_VISIBLE;
    style_ex = GetWindowLongPtr(hWnd, GWL_EXSTYLE) | (WS_EX_TOPMOST);
    hwndAfter = HWND_TOPMOST;
  }
  else
  {
    style = WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME;
    style_ex = GetWindowLongPtr(hWnd, GWL_EXSTYLE) & (~WS_EX_TOPMOST);
    hwndAfter = HWND_NOTOPMOST;
    AdjustWindowRect(&rc, style, false);
  }

  SetWindowLongPtr(hWnd, GWL_STYLE, style);
  SetWindowLongPtr(hWnd, GWL_EXSTYLE, style_ex);

  int desk_width  = GetSystemMetrics(SM_CXSCREEN);
  int desk_height = GetSystemMetrics(SM_CYSCREEN);

  int w = rc.right - rc.left;
  int h = rc.bottom - rc.top;
  int x = fullScreen ? 0 : (desk_width  - w) / 2;
  int y = fullScreen ? 0 : (desk_height - h) / 2;

  SetWindowPos(hWnd, hwndAfter, x, y, w, h, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}
//------------------------------------------------------------------------------
