
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "RaccoonRev.h"
#include "ChildView.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define GAMESYS_TIMER 3737

// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_ACTIVATE()
	ON_WM_CREATE()
	ON_WM_MOVE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return TRUE;
}



int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	/**********************************************************************/
	CMainFrame* pFrame = (CMainFrame*)GetParent();
	pFrame->RecalcLayout();
	// Get valid mainframe and client window rects 
	CRect rcFrm, rcClient;
	pFrame->GetWindowRect(&rcFrm);
	GetClientRect(&rcClient);
	// Calc non-client size of frame window 
	int ncwidth = rcFrm.Width() - rcClient.Width();
	int ncheight = rcFrm.Height() - rcClient.Height();
	// resize it 
	pFrame->SetWindowPos((const CWnd*)NULL, (int)0, (int)0,
		WND_SIZE_W + ncwidth,
		WND_SIZE_H + ncheight,
		SWP_NOZORDER | SWP_NOMOVE);

	// TODO:  Add your specialized creation code here
	m_gameSys = new CGameSys(this);

	SetTimer(GAMESYS_TIMER, 20, NULL);

	return 0;
}

void CChildView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);

	// TODO: Add your message handler code here

	// Activate/deactivate the game and update the Sleep status
	if (nState != WA_INACTIVE)
	{
		m_gameSys->GameActivate();
		m_gameSys->SetSleep(false);
	}
	else
	{
		m_gameSys->GameDeactivate();
		m_gameSys->SetSleep(true);
	}
}

void CChildView::OnMove(int x, int y)
{
	CWnd::OnMove(x, y);

	// TODO: Add your message handler code here
	m_gameSys->GameActivate();
}

void CChildView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// Paint the game
	m_gameSys->GamePaint(&dc);
}

void CChildView::OnDestroy()
{
	CWnd::OnDestroy();

	// End the game and exit the application
	KillTimer(3737);
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == GAMESYS_TIMER) {
		CClientDC dc(this);
		m_gameSys->GameCycle(&dc);
		m_gameSys->HandleKeys(&dc);
	}

	CWnd::OnTimer(nIDEvent);
}
