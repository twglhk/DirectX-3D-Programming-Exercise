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
	CUSTOMVERTEX vertices[] =
	{
		{ 150.0f,  50.0f, 0.5f, 1.0f, 0xffff0000, }, // x, y, z, rhw, color
		{ 250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00, },
		{  50.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff, },
	};

	m_pd3dDevice->CreateVertexBuffer( 3*sizeof(CUSTOMVERTEX), 0 , D3DFVF_CUSTOMVERTEX, 
		                              D3DPOOL_DEFAULT, &m_pVB, NULL );    

	VOID* pVertices;
	m_pVB->Lock( 0, sizeof(vertices), (void**)&pVertices, 0 );
	memcpy( pVertices, vertices, sizeof(vertices) );
	m_pVB->Unlock();
}

void CGameEdu01::OnRender()
{
	m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(CUSTOMVERTEX) );
	m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1 );
}

void CGameEdu01::OnUpdate()
{

}

void CGameEdu01::OnRelease()
{
	 if( m_pVB != NULL )        
         m_pVB->Release();
}