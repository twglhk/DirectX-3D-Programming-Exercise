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

}

void CGameEdu01::OnRender()
{
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
}

void CGameEdu01::OnUpdate()
{

}

void CGameEdu01::OnRelease()
{

}
