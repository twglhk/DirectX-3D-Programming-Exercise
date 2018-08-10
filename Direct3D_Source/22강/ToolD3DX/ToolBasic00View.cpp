
// ToolBasic00View.cpp : CToolBasic00View 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ToolBasic00.h"
#endif

#include "ToolBasic00Doc.h"
#include "ToolBasic00View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CToolBasic00View

IMPLEMENT_DYNCREATE(CToolBasic00View, CView)

BEGIN_MESSAGE_MAP(CToolBasic00View, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CToolBasic00View 생성/소멸

CToolBasic00View::CToolBasic00View()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	

}

CToolBasic00View::~CToolBasic00View()
{
	m_GameEdu01.Cleanup();
}

BOOL CToolBasic00View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolBasic00View 그리기

void CToolBasic00View::OnDraw(CDC* /*pDC*/)
{
	CToolBasic00Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}

void CToolBasic00View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CToolBasic00View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CToolBasic00View 진단

#ifdef _DEBUG
void CToolBasic00View::AssertValid() const
{
	CView::AssertValid();
}

void CToolBasic00View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolBasic00Doc* CToolBasic00View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolBasic00Doc)));
	return (CToolBasic00Doc*)m_pDocument;
}
#endif //_DEBUG


// CToolBasic00View 메시지 처리기


void CToolBasic00View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	m_GameEdu01.InitD3D(m_hWnd);
}


BOOL CToolBasic00View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return FALSE;
}