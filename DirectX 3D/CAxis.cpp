#include "stdafx.h"
#include "CAxis.h"


CAxis::CAxis()
{
}


CAxis::~CAxis()
{
}


void CAxis::OnInit(LPDIRECT3DDEVICE9 pd3dDevice)
{
	// Copy device
	m_pd3dDevice = pd3dDevice;

	// Vertices
	AXISVERTEX axis[] = {
		// X axis
		{ -3.0f, 0.0f, 0.0f, D3DCOLOR_RGBA(255,0,0,255)},
		{ 3.0f, 0.0f, 0.0f, D3DCOLOR_RGBA(255,0,0,255)},

		// Y axis
		{ 0.0f, -3.0f, 0.0f, D3DCOLOR_RGBA(0,255,0,255) },
		{ 0.0f, 3.0f, 0.0f, D3DCOLOR_RGBA(0,255,0,255) },

		// Z axis
		{ 0.0f, 0.0f, -3.0f, D3DCOLOR_RGBA(0,0,255,255) },
		{ 0.0f, 0.0f, 3.0f, D3DCOLOR_RGBA(0,0,255,255) },
	};

	// Create VertexBuffer
	m_pd3dDevice->CreateVertexBuffer(3 * sizeof(AXISVERTEX), 0, D3DFVF_AXISVERTEX, D3DPOOL_DEFAULT, &pVB, NULL);

	//Lock memory space for vertices
	VOID* pAxis;
	pVB->Lock(0, sizeof(axis), &pAxis, 0);
	memcpy(pAxis, axis, sizeof(axis));
	pVB->Unlock();
}


void CAxis::OnRender()
{
		D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld); // Init, use same coordinate system with model coordinate

	// Turn off the lighting
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Device stream setting for rendering
	m_pd3dDevice->SetStreamSource(0, pVB, 0, sizeof(AXISVERTEX));
	m_pd3dDevice->SetFVF(D3DFVF_AXISVERTEX);

	// World Transformation
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// Rendering
	m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, 3);
	
	// Turn on the lighting
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CAxis::OnRelease()
{
	if (pVB != NULL)
		pVB->Release();
}