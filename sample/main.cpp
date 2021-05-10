#include "stdafx.h"


typedef HRESULT(STDMETHODCALLTYPE* f_EndScene)(IDirect3DDevice9* pDevice); // Our function prototype 
f_EndScene oEndScene; // Original Endscene

HRESULT STDMETHODCALLTYPE Hooked_EndScene(IDirect3DDevice9* pDevice) // Our hooked endscene
{
    return oEndScene(pDevice);
}


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; // Buffer to hold Vertices

// A structure for our custom vertex type
struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw; // The transformed position for the vertex
	DWORD color;        // The vertex color
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)


//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D(HWND hWnd)
{
    // Create the D3D object.
    if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
        return E_FAIL;

    // Set up the structure used to create the D3DDevice
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

    if (FAILED(IDirect3D9_CreateDevice(g_pD3D, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
    {
        return E_FAIL;
    }

    // Device state would normally be set here

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InitVB()
// Desc: Creates a vertex buffer and fills it with our Vertices. The vertex
//       buffer is basically just a chuck of memory that holds Vertices. After
//       creating it, we must Lock()/Unlock() it to fill it. For indices, D3D
//       also uses index buffers. The special thing about vertex and index
//       buffers is that they can be created in device memory, allowing some
//       cards to process them in hardware, resulting in a dramatic
//       performance gain.
//-----------------------------------------------------------------------------
HRESULT InitVB()
{
    // Initialize three Vertices for rendering a triangle
    CUSTOMVERTEX Vertices[] =
    {
        { 150.0f,  50.0f, 0.5f, 1.0f, 0xffff0000, }, // x, y, z, rhw, color
        { 250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00, },
        {  50.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff, },
    };

    // Create the vertex buffer. Here we are allocating enough memory
    // (from the default pool) to hold all our 3 custom Vertices. We also
    // specify the FVF, so the vertex buffer knows what data it contains.
    if (FAILED(IDirect3DDevice9_CreateVertexBuffer(g_pd3dDevice, 3 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL)))
    {
        return E_FAIL;
    }

    // Now we fill the vertex buffer. To do this, we need to Lock() the VB to
    // gain access to the Vertices. This mechanism is required becuase vertex
    // buffers may be in device memory.
    VOID* pVertices;

    if (FAILED(IDirect3DVertexBuffer9_Lock(g_pVB, 0, sizeof(Vertices), (void**)&pVertices, 0)))
        return E_FAIL;

    memcpy(pVertices, Vertices, sizeof(Vertices));

    IDirect3DVertexBuffer9_Unlock(g_pVB);

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
#ifdef CINTERFACE
    if (g_pVB != NULL)
        g_pVB->lpVtbl->Release(g_pVB);

    if (g_pd3dDevice != NULL)
        g_pd3dDevice->lpVtbl->Release(g_pd3dDevice);

    if (g_pD3D != NULL)
        g_pD3D->lpVtbl->Release(g_pD3D);
#else
    if (g_pVB != NULL)
        IDirect3DVertexBuffer9_Release(g_pVB);

    if (g_pd3dDevice != NULL)
        IDirect3DDevice9_Release(g_pd3dDevice);

    if (g_pD3D != NULL)
        IDirect3D9_Release(g_pD3D);
#endif
}




//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID Render()
{
    // Clear the backbuffer to a blue color
    IDirect3DDevice9_Clear(g_pd3dDevice, 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

    // Begin the scene
    if (SUCCEEDED(IDirect3DDevice9_BeginScene(g_pd3dDevice)))
    {
        // Draw the triangles in the vertex buffer. This is broken into a few
        // steps. We are passing the Vertices down a "stream", so first we need
        // to specify the source of that stream, which is our vertex buffer. Then
        // we need to let D3D know what vertex shader to use. Full, custom vertex
        // shaders are an advanced topic, but in most cases the vertex shader is
        // just the FVF, so that D3D knows what type of Vertices we are dealing
        // with. Finally, we call DrawPrimitive() which does the actual rendering
        // of our geometry (in this case, just one triangle).
        IDirect3DDevice9_SetStreamSource(g_pd3dDevice, 0, g_pVB, 0, sizeof(CUSTOMVERTEX));
        IDirect3DDevice9_SetFVF(g_pd3dDevice, D3DFVF_CUSTOMVERTEX);
        IDirect3DDevice9_DrawPrimitive(g_pd3dDevice, D3DPT_TRIANGLELIST, 0, 1);

        // End the scene
        IDirect3DDevice9_EndScene(g_pd3dDevice);
    }

    // Present the backbuffer contents to the display
    IDirect3DDevice9_Present(g_pd3dDevice, NULL, NULL, NULL, NULL);
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        Cleanup();
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
// Name: wWinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
    UNREFERENCED_PARAMETER(hInst);

    // Register the window class
    WNDCLASSEX wc =
    {
        sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
        GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
        L"D3D Tutorial", NULL
    };
    RegisterClassEx(&wc);

    // Create the application's window
    HWND hWnd = CreateWindow(L"D3D Tutorial", L"D3D Tutorial 02: Vertices",
        WS_OVERLAPPEDWINDOW, 100, 100, 300, 300,
        NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (SUCCEEDED(InitD3D(hWnd)))
    {
        // Create the vertex buffer
        if (SUCCEEDED(InitVB()))
        {
            // Show the window
            ShowWindow(hWnd, SW_SHOWDEFAULT);
            UpdateWindow(hWnd);
            
#ifdef CINTERFACE
            oEndScene = g_pd3dDevice->lpVtbl->EndScene;
#else
            void** vtbl = *(reinterpret_cast<void***>(g_pd3dDevice));
            oEndScene = reinterpret_cast<f_EndScene>(vtbl[42]);
#endif

            DetourTransactionBegin();
            DetourUpdateThread(GetCurrentThread());
            DetourAttach(&oEndScene, Hooked_EndScene);
            DetourTransactionCommit();

            // Enter the message loop
            MSG msg;
            ZeroMemory(&msg, sizeof(msg));
            while (msg.message != WM_QUIT)
            {
                if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                else
                    Render();
            }
        }
    }

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&oEndScene, Hooked_EndScene);
    DetourTransactionCommit();

    UnregisterClass(L"D3D Tutorial", wc.hInstance);
    return 0;
}