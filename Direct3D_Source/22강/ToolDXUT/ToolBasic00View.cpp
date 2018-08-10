
// ToolBasic00View.cpp : CToolBasic00View 클래스의 구현
//

#include "stdafx.h"

#include <Strsafe.h>


// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ToolBasic00.h"
#endif

#include "ToolBasic00Doc.h"
#include "ToolBasic00View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CToolBasic00View

IMPLEMENT_DYNCREATE(CToolBasic00View, CView)

BEGIN_MESSAGE_MAP(CToolBasic00View, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
//	ON_WM_KEYDOWN()
//ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// 전역 함수들

//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
                                      bool bWindowed, void* pUserContext )
{
    // Typically want to skip back buffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                                         AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
                                         D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;	

    return true;
}


//--------------------------------------------------------------------------------------
// Before a device is created, modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
	pDeviceSettings->d3d9.pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                     void* pUserContext )
{
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                    void* pUserContext )
{	

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    HRESULT hr;

    // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 45, 50, 170 ), 1.0f, 0 ) );

    // Render the scene
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
		
        V( pd3dDevice->EndScene() );
    }
}


//--------------------------------------------------------------------------------------
// Handle messages to the application 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                          bool* pbNoFurtherProcessing, void* pUserContext )
{		
	
    return 0;
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9LostDevice( void* pUserContext )
{			
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
	
}

// CToolBasic00View 생성/소멸

CToolBasic00View::CToolBasic00View()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	

}

CToolBasic00View::~CToolBasic00View()
{
	DXUTShutdown();
}

BOOL CToolBasic00View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolBasic00View 그리기

void CToolBasic00View::OnDraw(CDC* /*pDC*/)
{
	CToolBasic00Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}

void CToolBasic00View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CToolBasic00View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CToolBasic00View 진단

#ifdef _DEBUG
void CToolBasic00View::AssertValid() const
{
	CView::AssertValid();
}

void CToolBasic00View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolBasic00Doc* CToolBasic00View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolBasic00Doc)));
	return (CToolBasic00Doc*)m_pDocument;
}
#endif //_DEBUG


// CToolBasic00View 메시지 처리기


void CToolBasic00View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	    // Set the callback functions
     DXUTSetCallbackD3D9DeviceAcceptable( IsD3D9DeviceAcceptable );
     DXUTSetCallbackD3D9DeviceCreated( OnD3D9CreateDevice );
     DXUTSetCallbackD3D9DeviceReset( OnD3D9ResetDevice );
     DXUTSetCallbackD3D9FrameRender( OnD3D9FrameRender );
     DXUTSetCallbackD3D9DeviceLost( OnD3D9LostDevice );
     DXUTSetCallbackD3D9DeviceDestroyed( OnD3D9DestroyDevice );
     DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );    
     DXUTSetCallbackFrameMove( OnFrameMove );

	 RECT rect;
	 DXUTInit( true, true); 
	 
	 DXUTSetWindow(m_hWnd, m_hWnd, m_hWnd, false );
	 GetClientRect(&rect);
	 DXUTCreateDevice( true, rect.right - rect.left, rect.bottom - rect.top);
}

LRESULT CToolBasic00View::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	LRESULT Result = CView::WindowProc(message, wParam, lParam);
	DXUTStaticWndProc(m_hWnd, message, wParam, lParam ); 
	return Result;
}
