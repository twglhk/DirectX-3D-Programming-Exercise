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
	m_Triangle.OnInit( m_pd3dDevice );
}

void CGameEdu01::OnRender()
{
	m_Triangle.OnRender();	
}

void CGameEdu01::OnUpdate()
{

}

void CGameEdu01::OnRelease()
{
	m_Triangle.OnRelease();
}