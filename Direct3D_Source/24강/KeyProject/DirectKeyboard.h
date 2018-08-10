#pragma once
#include <dinput.h>

class CDirectKeyboard
{
private:
	LPDIRECTINPUT8			m_pDI; // The DirectInput object         
	LPDIRECTINPUTDEVICE8	m_pKeyboard; // The keyboard device 
	HWND					m_hWnd;
	BYTE					m_Keys[256];

public:
	CDirectKeyboard(void);
	~CDirectKeyboard(void);

	HRESULT CreateDevice( HWND hWnd );	
	VOID	FreeDirectInput();
	void	OnActivate( WPARAM wParam );
	HRESULT ReadData();	
	BOOL	KeyDown( BYTE Key );
};
