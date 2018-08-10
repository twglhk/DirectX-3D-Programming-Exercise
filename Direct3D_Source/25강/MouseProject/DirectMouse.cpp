#include "DXUT.h"
#include "directmouse.h"

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

CDirectMouse::CDirectMouse(void)
{
	m_pMouse = NULL;
	m_pDI	 = NULL;	
}

CDirectMouse::~CDirectMouse(void)
{

}


HRESULT CDirectMouse::CreateDevice( HWND hWnd )
{
	HRESULT hr;

	m_hWnd = hWnd;
	if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
		IID_IDirectInput8, (VOID**)&m_pDI, NULL ) ) )
		return hr;

	if( FAILED( hr = m_pDI->CreateDevice( GUID_SysMouse, &m_pMouse, NULL ) ) )
		return hr;
	
	if( FAILED( hr = m_pMouse->SetDataFormat( &c_dfDIMouse2 ) ) )
		return hr;
	
	hr = m_pMouse->SetCooperativeLevel( m_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );

	if( FAILED(hr) )
		return hr;	
	
	m_pMouse->Acquire();	

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: FreeDirectInput()
// Desc: Initialize the DirectInput variables.
//-----------------------------------------------------------------------------
VOID CDirectMouse::FreeDirectInput()
{	
	if( m_pMouse ) 
		m_pMouse->Unacquire();
	
	SAFE_RELEASE( m_pMouse );
	SAFE_RELEASE( m_pDI );
}

//-----------------------------------------------------------------------------
// Name: ReadImmediateData()
// Desc: Read the input device's state when in immediate mode and display it.
//-----------------------------------------------------------------------------
HRESULT CDirectMouse::ReadData()
{
	HRESULT       hr;

	if( NULL == m_pMouse ) 
		return S_OK;

	ZeroMemory( &m_MouseState2, sizeof(m_MouseState2) );
	hr = m_pMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &m_MouseState2 );
	
	if( FAILED(hr) ) 
	{		
		hr = m_pMouse->Acquire();
		while( hr == DIERR_INPUTLOST ) 
			hr = m_pMouse->Acquire();
		
		return S_OK; 
	}	

	m_ptMouse.x = m_MouseState2.lX;
	m_ptMouse.y = m_MouseState2.lY;
	return S_OK;
}

BOOL CDirectMouse::PushLeft()
{
	return ( m_MouseState2.rgbButtons[0] & 0x80 ) ? TRUE : FALSE;	
}

BOOL CDirectMouse::PushRight()
{
	return ( m_MouseState2.rgbButtons[1] & 0x80 ) ? TRUE : FALSE;
}

BOOL CDirectMouse::PushMiddle()
{
	return ( m_MouseState2.rgbButtons[2] & 0x80 ) ? TRUE : FALSE;
}

void CDirectMouse::OnActivate( WPARAM wParam )
{	
	if( WA_INACTIVE != wParam && m_pMouse )
	{			
		 m_pMouse->Acquire();
	}	
}