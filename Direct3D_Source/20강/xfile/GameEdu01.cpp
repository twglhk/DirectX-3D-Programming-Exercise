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
	// 뷰와 프로젝트 셋업
	int i;

	RECT rect;
	D3DVIEWPORT9 vp;
	GetClientRect( m_hWnd, &rect );

	vp.X      = 0;
	vp.Y      = 0;
	vp.Width  = rect.right - rect.left;  // 좌표개념이 아니라 크기 개념 800, 600 계산됨
	vp.Height = rect.bottom - rect.top;
	vp.MinZ   = 0.0f;
	vp.MaxZ   = 1.0f;

	m_Eye.x = 3.0f;
	m_Eye.y = 0.1f;
	m_Eye.z = -5.0f;	

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


	LPD3DXBUFFER pD3DXMtrlBuffer;

    D3DXLoadMeshFromX( "Tiger.x", D3DXMESH_SYSTEMMEM, m_pd3dDevice, NULL, 
		                       &pD3DXMtrlBuffer, NULL, &m_dwNumMaterials, &m_pMesh );

	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials];
	m_pMeshTextures  = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];

	// 텍스쳐 로딩
	for( DWORD i = 0; i < m_dwNumMaterials; i++ )
	{
		m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

		// Set the ambient color for the material (D3DX does not do this)
		m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;

		m_pMeshTextures[i] = NULL;
		if( d3dxMaterials[i].pTextureFilename != NULL &&
				strlen( d3dxMaterials[i].pTextureFilename ) > 0 )
		{
		   D3DXCreateTextureFromFile( m_pd3dDevice, 	d3dxMaterials[i].pTextureFilename,
									   &m_pMeshTextures[i] );          
		}
	}

	pD3DXMtrlBuffer->Release();  // 머티리얼 버퍼 해제
	
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0xffffffff ); // 흰색 주변광
	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
}

void CGameEdu01::OnRender()
{
	 for( DWORD i = 0; i < m_dwNumMaterials; i++ )
	 {		
		  m_pd3dDevice->SetMaterial( &m_pMeshMaterials[i] );
		  m_pd3dDevice->SetTexture( 0, m_pMeshTextures[i] );

		  m_pMesh->DrawSubset( i );
	 }
}

void CGameEdu01::OnUpdate()
{

}

void CGameEdu01::OnRelease()
{
	if( m_pMeshMaterials != NULL )
		delete[] m_pMeshMaterials;

	 if( m_pMeshTextures )
	 {
		for( DWORD i = 0; i < m_dwNumMaterials; i++ )
		{
			 if( m_pMeshTextures[i] )
				m_pMeshTextures[i]->Release();
		}
		delete[] m_pMeshTextures;
	 }

	 if( m_pMesh != NULL )
		m_pMesh->Release(); 
}