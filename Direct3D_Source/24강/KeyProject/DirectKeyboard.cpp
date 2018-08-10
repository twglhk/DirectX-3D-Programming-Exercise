#include "DXUT.h"
#include ".\directkeyboard.h"

#define SAMPLE_BUFFER_SIZE 8  // arbitrary number of buffer elements

CDirectKeyboard::CDirectKeyboard(void)
{
	m_pDI       = NULL; // The DirectInput object         
	m_pKeyboard = NULL; // The keyboard device 
}

CDirectKeyboard::~CDirectKeyboard(void)
{
}

HRESULT CDirectKeyboard::CreateDevice( HWND hWnd )
{
	HRESULT hr;	

	m_hWnd = hWnd;	
	if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
		IID_IDirectInput8, (VOID**)&m_pDI, NULL ) ) )
		return hr;

	// 키보드 객체를 만든다.
	if( FAILED( hr = m_pDI->CreateDevice( GUID_SysKeyboard, &m_pKeyboard, NULL ) ) )
		return hr;
	
	// 사전정의 DirectInput 장치 데이터 구조체 c_dfDIKeyboard
	//extern const DIDATAFORMAT c_dfDIKeyboard;로 directinput.h에 정의되어 있음.
	if( FAILED( hr = m_pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) ) 
		return hr;

	// 협동 레벨이란 DirectInput을 사용하는 응용 프로그램과 시스템간의 상호 작용에 대한 설정
	hr = m_pKeyboard->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE 
		                                   | DISCL_NOWINKEY );
	
	if( FAILED(hr) )
		return hr;	
	
	m_pKeyboard->Acquire();
	
	return S_OK;
}

HRESULT CDirectKeyboard::ReadData()
{
	HRESULT hr;

	if( NULL == m_pKeyboard ) 
		return S_OK;
	
	ZeroMemory( m_Keys, sizeof(m_Keys) );
	hr = m_pKeyboard->GetDeviceState( sizeof(m_Keys), m_Keys );
	if( FAILED(hr) ) 
	{	
		hr = m_pKeyboard->Acquire();
		while( hr == DIERR_INPUTLOST ) 
			hr = m_pKeyboard->Acquire();		
		
		return S_OK; 
	}	
	
	return S_OK;
}

BOOL CDirectKeyboard::KeyDown( BYTE Key )
{	
	if( m_Keys[Key] & 0x80 ) 
		return TRUE;

	return FALSE;
}



//-----------------------------------------------------------------------------
// Name: FreeDirectInput()
// Desc: Initialize the DirectInput variables.
//-----------------------------------------------------------------------------
VOID CDirectKeyboard::FreeDirectInput()
{
	// Unacquire the device one last time just in case 
	// the app tried to exit while the device is still acquired.
	if( m_pKeyboard ) 
		m_pKeyboard->Unacquire();

	// Release any DirectInput objects.
	SAFE_RELEASE( m_pKeyboard );
	SAFE_RELEASE( m_pDI );
}

void CDirectKeyboard::OnActivate( WPARAM wParam )
{
	if( WA_INACTIVE != wParam && m_pKeyboard )
	{
		// Make sure the device is acquired, if we are gaining focus.
		m_pKeyboard->Acquire();
	}
}