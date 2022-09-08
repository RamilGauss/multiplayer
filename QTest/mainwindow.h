#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <Windows.h>

#include <qt/qmainwindow.h>
#include <qt/qlabel.h>
#include <d3d9.h>
#include <qtgui/QtGui>  

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9core.h>
#include <tchar.h>

#include "cube.h"
#include "input.h"

const int NumObjects = 1000;

#define D3DFVF_MYVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib") 
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

extern LPDIRECT3DDEVICE9 pd3dDevice;

class MainWindow:public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = 0,Qt::WindowFlags flags = 0);
    ~MainWindow();
    bool initDirect3D();
    void Init();
public slots:
    void Rendering();
protected:
    //void keyPressEvent(QKeyEvent *e);
private:
    bool isFull;
    LPDIRECT3D9 pD3D;
    LPD3DXFONT pFont;
    IDirect3DVertexBuffer9 *pVB;
    IDirect3DIndexBuffer9 *pIB;
    //LPDIRECTINPUT8 pInput;
    //LPDIRECTINPUTDEVICE8 pMouse;
    //DIMOUSESTATE mouse;
    TCHAR strFont[LF_FACESIZE];
    DWORD fontSize;
    int width;
    int height;
    Cube objects[NumObjects];
    //Cube cube;
    D3DXVECTOR3 coordEye;
    D3DXVECTOR3 coordLookAt;    // Point of look

    int numVerts;
    int numTriangles;
    int numInds;
    int sizeVertex;
    int sizeIndex;
    CustomVertex *verts;
    unsigned short *inds;
    int formatVertex;
    Input input;

    DWORD previousTime;
    DWORD currentTime;

    void SetView();
    void SetProjection();
    void DrawObject(Cube &cube);
    HRESULT FillBuffers(Cube &cube);
    void SetWorld(Cube &cube);
    HRESULT CreateD3DXFont(LPD3DXFONT* pFont, TCHAR* pstrFont, DWORD size);
    INT CalculateFPS();
    void InitialObjects();
    void ProcessingInput();
};

#endif