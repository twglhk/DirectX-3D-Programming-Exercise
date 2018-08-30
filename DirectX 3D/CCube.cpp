#include "stdafx.h"
#include "CCube.h"


CCube::CCube()
{
}


CCube::~CCube()
{
}

void CCube::OnInit(LPDIRECT3DDEVICE9 pDevice)
{
	// Init device;
	m_pDevice = pDevice;
	
	// Vertices data 
	CUBEVERTEX cCubeVertex[] =
	{
		// Upward face
		{ D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DCOLOR_RGBA(255,255,0,255) }, // v0
		{ D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DCOLOR_RGBA(255,0,0,255) }, // v1
		{ D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DCOLOR_RGBA(0,255,0,255) }, // v2
		{ D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DCOLOR_RGBA(255,0,255,255) }, // v3

		// Downward face
		{ D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DCOLOR_RGBA(255,0,255,255) }, // v4
		{ D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DCOLOR_RGBA(255,0,0,255) }, // v5
		{ D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DCOLOR_RGBA(255,255,0,255) }, // v6
		{ D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DCOLOR_RGBA(0,255,0,255) }, // v7
	};

	// Index data, 
	WORD indices[] =
	{
		0,1,2,
		0,2,3,
		4,0,3,
		4,3,7,
		7,3,2,
		7,2,6,
		6,2,1,
		6,1,5,
		5,1,0,
		5,0,4,
		5,4,7,
		5,7,6
	};

	// Create Vertex buffer and memory copy.
	m_pDevice->CreateVertexBuffer(sizeof(cCubeVertex), 0, D3DFVF_CUBEVERTEX, D3DPOOL_DEFAULT, &m_pVB, NULL);

	void* pCubevertices;
	m_pVB->Lock(0, sizeof(cCubeVertex), &pCubevertices, 0);
	memcpy(pCubevertices, cCubeVertex, sizeof(cCubeVertex));
	m_pVB->Unlock();

	// Create Index buffer and memory copy
	m_pDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIndexB, NULL);
	
	void* pIndices;
	m_pIndexB->Lock(0, sizeof(indices), &pIndices, 0);
	memcpy(pIndices, indices, sizeof(indices));
	m_pIndexB->Unlock();
}

void CCube::OnRender()
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld); // Init, use same coordinate system with model coordinate

	// Render state setup
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// World Transformation
	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// Device stream setting for rendering
	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(CUBEVERTEX));
	m_pDevice->SetIndices(m_pIndexB);
	m_pDevice->SetFVF(D3DFVF_CUBEVERTEX);

	// Rendering
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 6 * 2);

	// Render state recover 
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CCube::OnRelease()
{
	if (m_pVB != NULL)
		m_pVB->Release();

	if (m_pIndexB != NULL)
		m_pIndexB->Release();
}