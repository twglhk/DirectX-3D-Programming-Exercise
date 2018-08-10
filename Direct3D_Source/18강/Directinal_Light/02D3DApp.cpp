// 02D3DApp.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdio.h>
#include "02D3DApp.h"
#include "GameEdu01.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;								// 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.
HWND g_hWnd, g_hMenuWnd;
int g_nDlgWidth, g_nDlgHeight;

CGameEdu01 g_GameEdu01;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	MenuDlgProc( HWND , UINT , WPARAM , LPARAM );


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 여기에 코드를 입력합니다.
	MSG msg;

	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY02D3DAPP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	g_GameEdu01.InitD3D( g_hWnd );

	// 컨트롤 값 초기화 

	char string[10];
	sprintf( string, "%.1f", g_GameEdu01.m_Material.Diffuse.r );
	SetDlgItemText( g_hMenuWnd, IDC_EDIT1, string );
	sprintf( string, "%.1f", g_GameEdu01.m_Material.Diffuse.g );
	SetDlgItemText( g_hMenuWnd, IDC_EDIT2, string );
	sprintf( string, "%.1f", g_GameEdu01.m_Material.Diffuse.b );
	SetDlgItemText( g_hMenuWnd, IDC_EDIT3, string );

	sprintf( string, "%.1f", g_GameEdu01.m_Light.Diffuse.r );
	SetDlgItemText( g_hMenuWnd, IDC_EDIT4, string );
	sprintf( string, "%.1f", g_GameEdu01.m_Light.Diffuse.g );
	SetDlgItemText( g_hMenuWnd, IDC_EDIT5, string );
	sprintf( string, "%.1f", g_GameEdu01.m_Light.Diffuse.b );
	SetDlgItemText( g_hMenuWnd, IDC_EDIT6, string );
	sprintf( string, "%.1f", g_GameEdu01.m_Light.Specular.r );
	SetDlgItemText( g_hMenuWnd, IDC_EDIT7, string );
	sprintf( string, "%.1f", g_GameEdu01.m_Light.Specular.g );
	SetDlgItemText( g_hMenuWnd, IDC_EDIT8, string );
	sprintf( string, "%.1f", g_GameEdu01.m_Light.Specular.b );
	SetDlgItemText( g_hMenuWnd, IDC_EDIT9, string );
	sprintf( string, "%.1f", g_GameEdu01.m_Light.Ambient.r );
	SetDlgItemText( g_hMenuWnd, IDC_EDIT10, string );
	sprintf( string, "%.1f", g_GameEdu01.m_Light.Ambient.g );
	SetDlgItemText( g_hMenuWnd, IDC_EDIT11, string );
	sprintf( string, "%.1f", g_GameEdu01.m_Light.Ambient.b );
	SetDlgItemText( g_hMenuWnd, IDC_EDIT12, string );

	sprintf( string, "%.1f", g_GameEdu01.m_Light.Direction.x );
	SetDlgItemText( g_hMenuWnd, IDC_EDIT13, string );
	sprintf( string, "%.1f", g_GameEdu01.m_Light.Direction.y );
	SetDlgItemText( g_hMenuWnd, IDC_EDIT14, string );
	sprintf( string, "%.1f", g_GameEdu01.m_Light.Direction.z );
	SetDlgItemText( g_hMenuWnd, IDC_EDIT15, string );


	// 기본 메시지 루프입니다.
	while ( true )		
	{
		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE) )
		{
			if( msg.message == WM_QUIT )
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else{
			g_GameEdu01.Update();
			g_GameEdu01.Render();
		}				
	}

	return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
//  설명:
//
//    Windows 95에서 추가된 'RegisterClassEx' 함수보다 먼저
//    해당 코드가 Win32 시스템과 호환되도록
//    하려는 경우에만 이 함수를 사용합니다. 이 함수를 호출해야
//    해당 응용 프로그램에 연결된
//    '올바른 형식의' 작은 아이콘을 가져올 수 있습니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY02D3DAPP));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY02D3DAPP);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
   RECT rect = { 0, 0, 800, 600 };
   AdjustWindowRect( &rect, WS_OVERLAPPEDWINDOW , true);

   g_hWnd = hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, rect.right - rect.left , rect.bottom - rect.top, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   RECT rtWindow, rtDlg;
   GetWindowRect(g_hWnd, &rtWindow); // 스크린상에서 윈도우 좌표
    
   g_hMenuWnd = CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, MenuDlgProc ); 
   GetWindowRect( g_hMenuWnd, &rtDlg ); // 다이얼로그 크기
   g_nDlgWidth = rtDlg.right - rtDlg.left + 1; // 다이얼로그 가로 길이
   g_nDlgHeight = rtDlg.bottom - rtDlg.top + 1; // 다이얼로그 세로 길이
   
   MoveWindow(g_hMenuWnd, rtWindow.right, rtWindow.top, g_nDlgWidth, g_nDlgHeight, TRUE);   
   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rtWindow;

	switch (message)
	{	
	case WM_MOVE:
		GetWindowRect(hWnd, &rtWindow); // 스크린상에서 윈도우 좌표
		MoveWindow(g_hMenuWnd, rtWindow.right, rtWindow.top, g_nDlgWidth, g_nDlgHeight, TRUE);   
		break;	
	case WM_DESTROY:
		g_GameEdu01.Cleanup();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK MenuDlgProc( HWND hDlg, UINT nMsg, WPARAM wParam, 
                                    LPARAM lParam )
{
	 char str[10];
	 float fRed, fGreen, fBlue;
     
	 switch( nMsg )
     {
     case WM_INITDIALOG:		
             break;   
	 case WM_COMMAND:
		 if( wParam == IDC_BUTTON1 )  // 머티리얼 셋업
		 {
			 GetDlgItemText( hDlg, IDC_EDIT1, str, 9 );
			 fRed = atof(str);
			 GetDlgItemText( hDlg, IDC_EDIT2, str, 9 );
			 fGreen = atof(str);
			 GetDlgItemText( hDlg, IDC_EDIT3, str, 9 );
			 fBlue = atof(str);
			 g_GameEdu01.SetMaterial( fRed, fGreen, fBlue );
		 }else if( wParam == IDC_BUTTON2 ) // 라이트 셋업
		 {
			 GetDlgItemText( hDlg, IDC_EDIT4, str, 9 );
			 g_GameEdu01.m_Light.Diffuse.r = atof(str);
			 GetDlgItemText( hDlg, IDC_EDIT5, str, 9 );
			 g_GameEdu01.m_Light.Diffuse.g = atof(str);
			 GetDlgItemText( hDlg, IDC_EDIT6, str, 9 );
			 g_GameEdu01.m_Light.Diffuse.b = atof(str);

			 GetDlgItemText( hDlg, IDC_EDIT7, str, 9 );
			 g_GameEdu01.m_Light.Specular.r = atof(str);
			 GetDlgItemText( hDlg, IDC_EDIT8, str, 9 );
			 g_GameEdu01.m_Light.Specular.g = atof(str);
			 GetDlgItemText( hDlg, IDC_EDIT9, str, 9 );
			 g_GameEdu01.m_Light.Specular.b = atof(str);

			 GetDlgItemText( hDlg, IDC_EDIT10, str, 9 );
			 g_GameEdu01.m_Light.Ambient.r = atof(str);
			 GetDlgItemText( hDlg, IDC_EDIT11, str, 9 );
			 g_GameEdu01.m_Light.Ambient.g = atof(str);
			 GetDlgItemText( hDlg, IDC_EDIT12, str, 9 );
			 g_GameEdu01.m_Light.Ambient.b = atof(str);

			 GetDlgItemText( hDlg, IDC_EDIT13, str, 9 );
			 g_GameEdu01.m_Light.Direction.x = atof(str);
			 GetDlgItemText( hDlg, IDC_EDIT14, str, 9 );
			 g_GameEdu01.m_Light.Direction.y = atof(str);
			 GetDlgItemText( hDlg, IDC_EDIT15, str, 9 );
			 g_GameEdu01.m_Light.Direction.z = atof(str);

			 g_GameEdu01.SetDirectionalLight();
		 }
		 break;


     }
     return FALSE;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
