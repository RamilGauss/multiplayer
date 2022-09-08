

#include "mainwindow.h"

#include <d3d9.h> 
#include <d3dx9.h> 


MainWindow::MainWindow(QWidget* parent,Qt::WindowFlags flags):QMainWindow(parent, flags)
{
    coordEye = D3DXVECTOR3(0.0f, 0.0f, -100.0f);
    coordLookAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    isFull = false;
    pD3D = NULL;
    pd3dDevice = NULL;
    width = 1024;
    height = 768;
    resize(width, height);
    Cube cube;
    for(int i = 0; i < NumObjects; i++)
    {
        cube.SetCoord(rand() % 200 - 100, rand() % 200 - 100, rand() % 200 - 100);
        objects[i] = cube;
    }
    //input.Init(winId());
}

void MainWindow::Init()
{
    input.InitInput(winId());
}

MainWindow::~MainWindow()
{
    if(pIB != NULL)
        pIB->Release();
    if(pVB != NULL)
        pVB->Release();
    if(pFont != NULL)
        pFont->Release();
    if(pd3dDevice != NULL) 
        pd3dDevice->Release();
    if(pD3D != NULL)
        pD3D->Release();
    input.Shutdown();
}

// void MainWindow::keyPressEvent(QKeyEvent *e)
// {
//     switch(e->key())
//     {
//     case Qt::Key_W:
//     case Qt::Key_Up:
//         coordEye.z++;
//         coordLookAt.z++;
//         break;
//     case Qt::Key_S:
//     case Qt::Key_Down:
//         coordEye.z--;
//         coordLookAt.z--;
//         break;
//     case Qt::Key_A:
//     case Qt::Key_Left:
//         coordEye.x--;
//         coordLookAt.x--;
//         break;
//     case Qt::Key_D:
//     case Qt::Key_Right:
//         coordEye.x++;
//         coordLookAt.x++;
//         break;
//     case Qt::Key_Q:
//         coordEye.y--;
//         coordLookAt.y--;
//         break;
//     case Qt::Key_E:
//         coordEye.y++;
//         coordLookAt.y++;
//         break;
//     case Qt::Key_Escape:
//         close();
//         break;
//     default:
//         QWidget::keyPressEvent(e);
//     }
// }

bool MainWindow::initDirect3D()
{
  // Создание объекта Direct3D
  pD3D = Direct3DCreate9( D3D_SDK_VERSION);
  if( !pD3D)
    return false;
  
  // Создание устройства рендера.
  D3DPRESENT_PARAMETERS d3dpp = {0};
  d3dpp.Windowed = TRUE;
  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
//   d3dpp.FullScreen_RefreshRateInHz = 0;
//   d3dpp.PresentationInterval = 0;
  d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
  d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  d3dpp.EnableAutoDepthStencil = TRUE;
  d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
  HRESULT hr;
  hr = pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, winId(),
                           D3DCREATE_HARDWARE_VERTEXPROCESSING,
                           &d3dpp, &pd3dDevice );
  if( FAILED(hr) || !pd3dDevice)
    return false;
    
/*    if(NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
        return false;
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    if(isFull)
    {
        d3dpp.BackBufferWidth = width;
        d3dpp.BackBufferHeight = height;
        d3dpp.BackBufferCount = 1;
        d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
    }
    else
    {
//         d3dpp.FullScreen_RefreshRateInHz = 0;
//         d3dpp.PresentationInterval = 0;
    }
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
//     d3dpp.EnableAutoDepthStencil = true;
//     d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.Windowed = !isFull;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, winId(),
        D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pd3dDevice)))
        return false;*/
    CreateD3DXFont(&pFont, strFont, fontSize);
    pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
    pd3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
    SetView();
    SetProjection();
    InitialObjects();
    return true;
}

void MainWindow::Rendering()
{
    ProcessingInput();
    static char buffer[256];
    RECT rect = {0, 0, 300, 300};
    sprintf_s(buffer, 255, "Current FPS - %d", CalculateFPS());
    SetView();
    pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    pd3dDevice->BeginScene();
    for(UINT i = 0; i < NumObjects; i++)
    {
        DrawObject(objects[i]);
    }
    //DrawObject(cube);
    pFont->DrawTextA(0, buffer, -1, &rect, DT_TOP | DT_LEFT, D3DCOLOR_XRGB(0, 127, 255));
    pd3dDevice->EndScene();
    pd3dDevice->Present(NULL, NULL, NULL, NULL); 
}

void MainWindow::SetView()
{
  D3DXMATRIX matrixView;
  D3DXMatrixLookAtLH(&matrixView, &coordEye,
      &coordLookAt, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
  pd3dDevice->SetTransform(D3DTS_VIEW, &matrixView);
}

void MainWindow::SetProjection()
{
    D3DXMATRIX matrixProjection;
    D3DXMatrixPerspectiveFovLH(&matrixProjection, D3DX_PI/4, 1, 1, 1000.0f);
    pd3dDevice->SetTransform(D3DTS_PROJECTION, &matrixProjection);
}

void MainWindow::DrawObject(Cube &cube)
{
  // выставляем матрицу для объекта в мировых координатах
  SetWorld(cube);
  // устанавливаем вершинный формат
  pd3dDevice->SetFVF( formatVertex );
  // устанавливаем вершинный буфер
  pd3dDevice->SetStreamSource( 0, pVB, 0, sizeVertex );

  // устанавливаем индексный буфер
  pd3dDevice->SetIndices(pIB);

  // вывод объекта
  pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, NumVerts, 0, NumTriangles);
}
void MainWindow::InitialObjects()
{
    formatVertex = objects[0].GetInformation(&verts, &inds, numVerts, numTriangles, numInds, sizeVertex, sizeIndex);
    if(FAILED(pd3dDevice->CreateVertexBuffer(numVerts * sizeVertex, 0, formatVertex, D3DPOOL_DEFAULT, &pVB, NULL)))
        return;
    VOID *pv;
    if(FAILED(pVB->Lock(0, sizeVertex * numVerts, (void**)&pv, 0)))
        return;
    memcpy(pv, verts, sizeVertex * numVerts);
    pVB->Unlock();

    if(FAILED(pd3dDevice->CreateIndexBuffer(numInds * sizeIndex, 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &pIB, NULL)))
        return;
    VOID *pi;
    if(FAILED(pIB->Lock(0, numInds * sizeIndex, (void**)&pi, 0)))
        return;
    memcpy(pi, inds, sizeIndex * numInds);
    pIB->Unlock();
    return;
}

void MainWindow::SetWorld(Cube &cube)
{
  D3DXMATRIX world;
  D3DXMATRIX rotationX;
  D3DXMATRIX rotationY;
  D3DXMATRIX translate;
  UINT time = GetTickCount() % 2500;
  FLOAT angle = time * (2.0f * D3DX_PI) / 2500.0f;
  D3DXMatrixRotationX(&rotationX, angle);
  D3DXMatrixRotationY(&rotationY, angle);
  D3DXMatrixTranslation(&translate, cube.x, cube.y, cube.z);
  world = rotationX * rotationY * translate;
  pd3dDevice->SetTransform(D3DTS_WORLD, &world);
}

HRESULT MainWindow::CreateD3DXFont(LPD3DXFONT* pFont, TCHAR* pstrFont, DWORD size)
{
    HRESULT hr;
    LPD3DXFONT pFontNew = NULL;
//    HDC hDC;
    INT height = 20;

//      hDC = GetDC(NULL);
//      height = -MulDiv(size, GetDeviceCaps(hDC, LOGPIXELSY), 72);
//      ReleaseDC(NULL, hDC);

    hr = D3DXCreateFont(pd3dDevice, height, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, pstrFont, &pFontNew);
    if(SUCCEEDED(hr))
        *pFont = pFontNew;
    return hr;
}

INT MainWindow::CalculateFPS()
{
    static INT FPS = 0;
    static INT nFrames = 0;
    INT timeFrames;
    nFrames++;
    if(timeFrames = ( (currentTime = GetTickCount()) - previousTime) >= 1000)
    {
        FPS = nFrames / timeFrames;
        previousTime = currentTime;
        nFrames = 0;
    }
    return FPS;
}

void MainWindow::ProcessingInput()
{
    input.Update();
    coordEye.x += input.GetMouseSpeedX()/10.0f;
    coordLookAt.x += input.GetMouseSpeedX()/10.0f;
    if(input.isKey(DIK_ESCAPE))
         close();
}  