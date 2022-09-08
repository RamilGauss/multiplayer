//
// fxd - a D3DX effect disassembler
//
// (c) by Wraith, 2k8
//

#include <d3dx9.h>
#include <stdio.h>
#include <assert.h>
#include <DxErr.h>

#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "dxerr.lib" )


static char const* class_name = "wra.FXD.class";

IDirect3D9* g_d3d             = 0;
IDirect3DDevice9* g_d3ddev    = 0;
D3DPRESENT_PARAMETERS g_d3dpp = {0,};

#ifdef _DEBUG
#define LOG printf
#else
#define LOG
#endif

#define dxerr DXGetErrorString
#define SAFE_RELEASE(x) if(x){ x->Release(); x = 0; } else {}

//////////////////////////////////////////////////////////////////////////


LRESULT APIENTRY wndproc( HWND, UINT, WPARAM, LPARAM );


//////////////////////////////////////////////////////////////////////////

static void reg_class()
{
    WNDCLASSA wc;
    LOG("  window class\n");
    wc.cbClsExtra = wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    wc.hCursor = LoadCursorA( 0, IDC_ARROW );
    wc.hIcon =   LoadIconA( 0, IDI_APPLICATION );
    wc.hInstance = GetModuleHandleA(0);
    wc.lpfnWndProc = wndproc;
    wc.lpszClassName = class_name;
    wc.lpszMenuName = 0;
    wc.style = 0;
    RegisterClassA( &wc );
}

static HWND create_wnd()
{
    LOG("  window\n");
    return CreateWindowEx( 0, class_name, "FXD", WS_OVERLAPPEDWINDOW, 20, 20, 800,600, 0, 0, GetModuleHandleA(0), 0 );
}

static void init_d3dpp( HWND hwnd )
{
    LOG("  d3d present param\n");
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
    g_d3dpp.BackBufferCount        = 1;
    g_d3dpp.BackBufferFormat       = D3DFMT_X8R8G8B8;
    g_d3dpp.BackBufferWidth        = 32;
    g_d3dpp.BackBufferHeight       = 32;
    g_d3dpp.EnableAutoDepthStencil = FALSE;
    g_d3dpp.Flags = 0; //D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
    g_d3dpp.FullScreen_RefreshRateInHz = 0;
    g_d3dpp.hDeviceWindow = hwnd;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.Windowed = TRUE;
}


static HRESULT init_d3d( HWND hwnd )
{
    HRESULT hr;
    DWORD device_flags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    int N = 0;

    LOG("  d3d object\n");
    g_d3d = Direct3DCreate9( D3D_SDK_VERSION );
    assert( g_d3d );

    init_d3dpp( hwnd );
    LOG("  d3d device (NULLREF)\n");
    if( FAILED( hr = IDirect3D9_CreateDevice( g_d3d, N, D3DDEVTYPE_NULLREF, hwnd, device_flags, &g_d3dpp, &g_d3ddev ) ) )
    {
        LOG("Error: could not create NULLREF device. [%s]", dxerr(hr) );
        return hr; 
    }

    return S_OK;
}

int gfx_init()
{
    HWND hwnd = 0;
    HRESULT hr;

    LOG("gfx init\n");

    reg_class();
    hwnd = create_wnd();
    assert( hwnd );

    if( FAILED( hr = init_d3d( hwnd ) ) )
        return 0;

    LOG("gfx init ok\n");
    return 1;
}

void gfx_cleanup()
{
    SAFE_RELEASE( g_d3ddev );
    SAFE_RELEASE( g_d3d );
}

LRESULT APIENTRY wndproc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp )
{
    switch( msg )
    {
    case WM_DESTROY:
        PostQuitMessage( 0 );
        return 0;
    }
    return DefWindowProc( hwnd, msg, wp, lp );
}



int main( int argc, char* argv[] )
{
    HRESULT hr = 0;
    ID3DXEffect* eff = 0;
    ID3DXBuffer* err = 0;
    ID3DXBuffer* dasm = 0;

    if( !argv[1] )
    {
        return fprintf(stderr,"Usage: fxd <fx-file>\n");
    }

    gfx_init();
    
    hr = D3DXCreateEffectFromFile( g_d3ddev, argv[1], 0, 0, 0, 0, &eff, &err );
    if( err )
    {
        fprintf(stderr,"%s\n", err->GetBufferPointer() );
    }

    D3DXDisassembleEffect( eff, FALSE, &dasm );
    //FILE* vf = fopen( "out.txt", "w" );
    fprintf( stdout, "%s\n", dasm->GetBufferPointer() );
    //fclose(vf);

    fprintf(stderr,"%s\n", argv[1] );

}

