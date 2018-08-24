#include "stdafx.h"
#include "CTriangle.h"


CTriangle::CTriangle()
{
}


CTriangle::~CTriangle()
{
}

void CTriangle::OnInit(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;

	// Vertex info
	CUSTOMVERTEX vertices[3] =
	{
		{ 150.0f, 50.0f, 0.5f, 1.0f, 0xffff0000, },
	{ 250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00, },
	{ 50.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff, },
	};

	// CreateVertexBuffer for draw Primitive
	m_pd3dDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &pVB, NULL);

	// Memory Lock for Vertex Buffer
	VOID* pVertices;
	pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0);
	memcpy(pVertices, vertices, sizeof(vertices));
	pVB->Unlock();
}

void CTriangle::OnRender()
{
	// Device stream setting for rendering
	m_pd3dDevice->SetStreamSource(0, pVB, 0, sizeof(CUSTOMVERTEX));
	m_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
}

void CTriangle::OnUpdate()
{
	// Changing of vertex position
}

void CTriangle::OnRelease()
{
	// Release Vertex Buffer
	if (pVB != NULL)
		pVB->Release();
}