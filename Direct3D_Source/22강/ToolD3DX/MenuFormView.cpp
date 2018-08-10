// MenuFormView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ToolBasic00.h"
#include "MenuFormView.h"


// CMenuFormView

IMPLEMENT_DYNCREATE(CMenuFormView, CFormView)

CMenuFormView::CMenuFormView()
	: CFormView(CMenuFormView::IDD)
{

}

CMenuFormView::~CMenuFormView()
{
}

void CMenuFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMenuFormView, CFormView)
END_MESSAGE_MAP()


// CMenuFormView 진단입니다.

#ifdef _DEBUG
void CMenuFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMenuFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMenuFormView 메시지 처리기입니다.



