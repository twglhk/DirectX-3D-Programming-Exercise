##pragma once
#include <d3dx9.h>

class CGround
{
	DWORD m_dwNumVertices, m_dwNumIndices; // 정점의 개수
	LPDIRECT3DINDEXBUFFER9	m_pIB;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DDEVICE9		m_pd3dDevice;
	int m_nCol, m_nRow;

public:
	void Create(LPDIRECT3DDEVICE9 pd3dDevice, int nRow, int nCol, float fSize); // Size : 정점 사이의 거리
	void OnRender();
	void OnRelease();

public:
	CGround(void);
	~CGround(void);
};

