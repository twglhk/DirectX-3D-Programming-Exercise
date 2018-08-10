#include "StdAfx.h"
#include "GameEdu01.h"


CGameEdu01::CGameEdu01(void)
{
}


CGameEdu01::~CGameEdu01(void)
{
}

void CGameEdu01::OnInit()
{
	// ºä¿Í ÇÁ·ÎÁ§Æ® ¼Â¾÷
	int i;

	RECT rect;
	D3DVIEWPORT9 vp;
	GetClientRect( m_hWnd, &rect );

	vp.X      = 0;
	vp.Y      = 0;
	vp.Width  = rect.right - rect.left;  // ÁÂÇ¥°³³äÀÌ ¾Æ´Ï¶ó Å©±â °³³ä 800, 600 °è»êµÊ
	vp.Height = rect.bottom - rect.top;
	vp.MinZ   = 0.0f;
	vp.MaxZ   = 1.0f;

	m_Eye.x = 0.0f;
	m_Eye.y = 0.1f;
	m_Eye.z = -10.0f;	

	m_At.x = 0.0f;
	m_At.y = 0.0f;
	m_At.z = 0.0f;

	m_Up.x = 0.0f;
	m_Up.y = 1.0f;
	m_Up.z = 0.0f;
	
	D3DXMatrixLookAtLH( &m_matView, &m_Eye, &m_At, &m_Up );
	m_pd3dDevice->SetTransform( D3DTS_VIEW, &m_matView );	
    
    D3DXMatrixPerspectiveFovLH( &m_matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f );
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProj );
	m_pd3dDevice->SetViewport( &vp );

	// »ï°¢Çü ¼³Á¤
	CUSTOMVERTEX vertices[] = { 
		{ D3DXVECTOR3( -1.0f, 1.0f, 0.0f ), 0.0f, 0.0f },
		{ D3DXVECTOR3(  1.0f, 1.0f, 0.0f ), 1.0f, 0.0f },
		{ D3DXVECTOR3( -1.0f, -1.0f, 0.0f ), 0.0f, 1.0f },
		{ D3DXVECTOR3(  1.0f, -1.0f, 0.0f ), 1.0f, 1.0f },	
	};

	m_pd3dDevice->CreateVertexBuffer( sizeof(vertices), 0 , D3DFVF_CUSTOMVERTEX, 
		                              D3DPOOL_DEFAULT, &m_pVB, NULL );    
		
	VOID* pVertices;
	m_pVB->Lock( 0, sizeof(vertices), (void**)&pVertices, 0 );
	memcpy( pVertices, vertices, sizeof(vertices) );
	m_pVB->Unlock();

	D3DXCreateTextureFromFile( m_pd3dDevice, "banana.bmp", &m_pTexture );

	D3DXMatrixScaling( &m_matScale[0], 0.5f, 0.5f, 0.5f );
	D3DXMatrixScaling( &m_matScale[1], 1.5f, 1.5f, 1.5f );

	D3DXMatrixTranslation( &m_matTranslate[0], -2.0f, 0.0f, 0.0f );
	D3DXMatrixTranslation( &m_matTranslate[1], 1.5f, 0.0f, 0.0f );
	
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
}

void CGameEdu01::OnRender()
{
	D3DXMATRIX matWorld;

	m_pd3dDevice->SetTexture( 0, m_pTexture);
	m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(CUSTOMVERTEX) );
	m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

	for( int i = 0 ; i < 2 ; i++ )
	{
		matWorld = m_matScale[i] * m_matTranslate[i];
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld );
		m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );	
	}
}

void CGameEdu01::OnUpdate()
{

}

void CGameEdu01::OnRelease()
{
	if( m_pTexture != NULL )
		m_pTexture->Release();

	 if( m_pVB != NULL )        
         m_pVB->Release();	 
}