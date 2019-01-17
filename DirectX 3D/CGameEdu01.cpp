#include "stdafx.h"
#include "CGameEdu01.h"


CGameEdu01::CGameEdu01()
{

}


CGameEdu01::~CGameEdu01()
{
	
}


void CGameEdu01::OnInit()
{
	RECT rect;
	D3DVIEWPORT9 vp;

	// Init view trans param
	m_Eye.x = 10.0f;
	m_Eye.y = 15.0f;
	m_Eye.z = -10.0f;

	m_At.x = 0.0f;
	m_At.y = 0.0f;
	m_At.z = 0.0f;

	m_Up.x = 0.0f;
	m_Up.y = 1.0f;
	m_Up.z = 0.0f;

	// Viewport param setup
	GetClientRect(m_hWnd, &rect);
	
	vp.X = 0;
	vp.Y = 0;
	vp.Width = rect.right - rect.left;
	vp.Height = rect.bottom - rect.top;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;

	// View transformation
	D3DXMatrixLookAtLH(&m_matView, &m_Eye, &m_At, &m_Up);
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_matView);

	// Projection transformation
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

	// Viewport setup 
	m_pd3dDevice->SetViewport(&vp);

	// Object Setup
	//triangle->OnInit(m_pd3dDevice);
	axis.OnInit(m_pd3dDevice);
	//cube.OnInit(m_pd3dDevice);

	//D3DXCreateBox(m_pd3dDevice, 2.0f, 2.0f, 2.0f, &m_pBoxMesh, NULL); //box mesh setup
	D3DXCreateTeapot(m_pd3dDevice, &m_pTeapotMesh, NULL);
	//D3DXCreateSphere(m_pd3dDevice, 3.0f, 20, 20,  &m_pSphereMesh, NULL);
	//D3DXCreateCylinder(m_pd3dDevice, 1.0f, 1.0f, 5.0f, 30, 30, &m_pCylinderMesh, NULL);

	m_fScale = 1.0f;
}

void CGameEdu01::OnRender()
{
	D3DXMATRIX matScale;
	D3DXMATRIX matRot;
	D3DXMATRIX matTrans;
	D3DXMATRIX matConcat1;
	D3DXMATRIX matConcat2;

	D3DXVECTOR3 vecRotAxis = { 0.0f, 1.0f, 0.0f };
	D3DXQUATERNION vQuart;
	float x, y, z;

	x = D3DXToRadian(45);
	y = D3DXToRadian(90);
	z = D3DXToRadian(45);

	//triangle->OnRender();
	
	axis.OnRender();
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	

	// Scale
	D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
	
	
	// Rotation

	D3DXMatrixRotationY(&matRot, GetTickCount() * 0.004f);
	//D3DXMatrixRotationAxis(&matRot, &vecRotAxis, GetTickCount() * 0.004f);
	//D3DXQuaternionRotationAxis(&vQuart, &vecRotAxis, GetTickCount() * 0.004f);
	//D3DXMatrixRotationQuaternion(&matRot, &vQuart);
	//D3DXMatrixRotationYawPitchRoll(&matRot, y, x, z);
	//D3DXQuaternionRotationYawPitchRoll(&vQuart, y, x, z);
	//D3DXMatrixRotationQuaternion(&matRot, &vQuart);

	// Translation
	D3DXMatrixTranslation(&matTrans, 5.0f, 0.0f, 0.0f);

	// Concatenation
	//matConcat = matScale * matRot * matTrans;
	matConcat1 = matScale * matTrans * matRot;

	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matConcat1);
	//m_pBoxMesh->DrawSubset(0);
	//m_pTeapotMesh->DrawSubset(0);
	//m_pCylinderMesh->DrawSubset(0);
	//m_pSphereMesh->DrawSubset(0);

	
	//m_pd3dDevice->SetTransform(D3DTS_WORLD, &matRotX);
	m_pTeapotMesh->DrawSubset(0);


	// Draw second Teapot
	D3DXMatrixTranslation(&matTrans, 2.0f, 0.0f, 0.0f);
	matConcat2 = matScale * matRot * matTrans;
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matConcat2);
	m_pTeapotMesh->DrawSubset(0);

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//cube.OnRender();

}

void CGameEdu01::OnUpdate()
{
	// Changing of vertex position
	if (GetAsyncKeyState('A') < 0)
	{
		m_fScale -= 0.1f;
	}
		
	if (GetAsyncKeyState('D') < 0)
		m_fScale += 0.1f;

	//std::cout << "OnUpdate" << std::endl;
}

void CGameEdu01::OnRelease()
{
	//triangle->OnRelease();
	axis.OnRelease();
	m_pTeapotMesh->Release();
	//cube.OnRelease();
}
