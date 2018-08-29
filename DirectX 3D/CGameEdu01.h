#pragma once
#include "CD3DApp.h"


class CGameEdu01 : public CD3DApp
{
	CTriangle triangle;
	CAxis axis;

	D3DXMATRIX m_matView, m_matProj; // matrix 
	D3DXVECTOR3 m_Eye, m_At, m_Up; // View vectors

	virtual void OnInit();
	virtual void OnRender();
	virtual void OnUpdate();
	virtual void OnRelease();

public:
	CGameEdu01();
	~CGameEdu01();
};

