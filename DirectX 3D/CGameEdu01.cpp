#include "stdafx.h"
#include "CGameEdu01.h"


CGameEdu01::CGameEdu01()
{
	triangle = new CTriangle();
}


CGameEdu01::~CGameEdu01()
{
}


void CGameEdu01::OnInit()
{
	triangle->OnInit(m_pd3dDevice);
}

void CGameEdu01::OnRender()
{
	triangle->OnRender();
}

void CGameEdu01::OnUpdate()
{
	// Changing of vertex position
}

void CGameEdu01::OnRelease()
{
	triangle->OnRelease();
}
