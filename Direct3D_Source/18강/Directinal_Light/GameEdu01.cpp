#include "StdAfx.h"
#include "GameEdu01.h"


CGameEdu01::CGameEdu01(void)
{
}


CGameEdu01::~CGameEdu01(void)
{
}

void CGameEdu01::SetMaterial( float r, float g, float b )
{	
	D3DCOLORVALUE Diffuse;
	Diffuse.r = r;
	Diffuse.g = g;
	Diffuse.b = b;
	Diffuse.a = 1.0f;
	ZeroMemory( &m_Material, sizeof(D3DMATERIAL9) );
	m_Material.Ambient = m_Material.Diffuse = Diffuse;
	m_pd3dDevice->SetMaterial( &m_Material );
}


void CGameEdu01::SetDirectionalLight()
{
	m_pd3dDevice->SetLight(0, &m_Light);
	m_pd3dDevice->LightEnable(0, TRUE);
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

	m_Eye.x = 0.0f;
	m_Eye.y = 4.0f;
	m_Eye.z = -4.0f;	

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
	
	// 삼각형 설정
	CUSTOMVERTEX vertices[] = { 
		{ D3DXVECTOR3(-1.0f, -0.3f, 0.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(-1.0f, 0.0f, 1.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) },
		
		{ D3DXVECTOR3(-1.0f, 0.0f, 1.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) },

		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(1.0f, -0.3f, 0.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) },

		{ D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(1.0f, 0.0f, 1.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(1.0f, -0.3f, 0.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) },

		{ D3DXVECTOR3(-1.0f, 0.0f, -1.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(-1.0f, -0.3f, 0.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) },

		{ D3DXVECTOR3(-1.0f, -0.3f, 0.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) },

		{ D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(1.0f, 0.0f, -1.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) },

		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(1.0f, -0.3f, 0.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) },
		{ D3DXVECTOR3(1.0f, 0.0f, -1.0f), D3DXVECTOR3( 0.0f, 0.0f, 0.0f) }
	};

	m_pd3dDevice->CreateVertexBuffer( sizeof(vertices), 0 , D3DFVF_CUSTOMVERTEX, 
		                              D3DPOOL_DEFAULT, &m_pVB, NULL );    

	// 법선 벡터 계산
	D3DXVECTOR3 vS0, vS1;
	for( i = 0 ; i < 8 ; i++ )
	{
		vS0 = vertices[i*3 + 2].vPos - vertices[i*3].vPos;
		vS1 = vertices[i*3 + 1].vPos - vertices[i*3].vPos;
		D3DXVec3Cross( &vertices[i*3].vNormal, &vS1, &vS0 );
		D3DXVec3Normalize( &vertices[i*3].vNormal,  &vertices[i*3].vNormal );

		// 하나의 삼각형을 구성하는 법선벡터는 모두 같다. 
		vertices[i*3 + 2].vNormal = vertices[i*3 + 1].vNormal = vertices[i*3].vNormal;
	}
	
	VOID* pVertices;
	m_pVB->Lock( 0, sizeof(vertices), (void**)&pVertices, 0 );
	memcpy( pVertices, vertices, sizeof(vertices) );
	m_pVB->Unlock();

	// 법선 벡터 출력을 위한 라인 == 7강의 축 클래스를 참조하면 쉽다.
	CROSSVERTEX vCrossVertices[48];

	for( i = 0 ; i < 24 ; i++ )
	{
		vCrossVertices[i*2].vPos = vertices[i].vPos;
		vCrossVertices[(i*2) + 1].vPos = vertices[i].vPos + vertices[i].vNormal;

		vCrossVertices[(i*2) + 1].dwDiffuse = vCrossVertices[i*2].dwDiffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	}

	m_pd3dDevice->CreateVertexBuffer( sizeof(vCrossVertices), 0 , D3DFVF_CROSSVERTEX, 
		                              D3DPOOL_DEFAULT, &m_pCrossVB, NULL );    
	
	m_pCrossVB->Lock( 0, sizeof(vertices),&pVertices, 0 );
	memcpy( pVertices, vCrossVertices, sizeof(vCrossVertices) );
	m_pCrossVB->Unlock();

	// 머티리얼 설정

	SetMaterial( 1.0f, 1.0f, 1.0f );

	// 라이트 기본 초기화
	m_Light.Type = D3DLIGHT_DIRECTIONAL;
	m_Light.Diffuse.r = m_Light.Diffuse.g = m_Light.Diffuse.b = m_Light.Diffuse.a = 1.0f;
	
	m_Light.Specular =  m_Light.Diffuse;
	m_Light.Ambient.g = m_Light.Ambient.b = m_Light.Ambient.r = 0.7f;
	m_Light.Ambient.a = 1.0f;
	m_Light.Direction.x = -1.0f;
	m_Light.Direction.y = -0.4f;
	m_Light.Direction.z = 0.5f;

	SetDirectionalLight();	
}


void CGameEdu01::OnRender()
{	
	m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(CUSTOMVERTEX) );
	m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 8 );

	// 법선 출력
	/*m_pd3dDevice->SetFVF( D3DFVF_CROSSVERTEX );	
	m_pd3dDevice->SetStreamSource( 0, m_pCrossVB, 0, sizeof(CROSSVERTEX) );
	m_pd3dDevice->DrawPrimitive( D3DPT_LINELIST, 0, 24 );	*/
}

void CGameEdu01::OnUpdate()
{

}

void CGameEdu01::OnRelease()
{
	 if( m_pVB != NULL )        
         m_pVB->Release();

	  if( m_pCrossVB != NULL )        
         m_pCrossVB->Release();
}