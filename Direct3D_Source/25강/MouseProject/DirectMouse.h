#pragma once
#include <dinput.h>

class CDirectMouse
{	
	LPDIRECTINPUT8       m_pDI;       
	LPDIRECTINPUTDEVICE8 m_pMouse;
	DIMOUSESTATE2		 m_MouseState2;      // DirectInput mouse state structure
	HWND				 m_hWnd;

public:
	CDirectMouse(void);
	~CDirectMouse(void);

	HRESULT CreateDevice( HWND hWnd );
	HRESULT ReadData();
	VOID	FreeDirectInput();
	void	OnActivate( WPARAM wParam );
	POINT	m_ptMouse;
	BOOL	PushLeft();
	BOOL	PushRight();
	BOOL	PushMiddle();
};


