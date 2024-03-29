
// MainFrm.cpp : CMainFrame Å¬·¡½ºÀÇ ±¸Çö
//

#include "stdafx.h"
#include "Game.h"

#include "MainFrm.h"
#include "GameDoc.h"
#include "GameView.h"
#include "InGameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WND_X 1200
#define WND_Y 800

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
END_MESSAGE_MAP()


CMainFrame::CMainFrame()
{
	// TODO:
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs

	if (cs.hMenu != NULL) {
	::DestroyMenu(cs.hMenu);	// delete menu if loaded
	cs.hMenu = NULL;			// no menu for this window
	}

	cs.x = 10;
	cs.y = 20;

	cs.cx = 1200;				// 가로 크기
	cs.cy = 800;				// 세로 크기

	return TRUE;
}

// CMainFrame Áø´Ü

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG



void CMainFrame::SwitchView(eView nView)
{
    CView* pOldActiveView = GetActiveView();			// 현재 뷰
    CView* pNewActiveView = (CView*) GetDlgItem(nView);	// 생성할 뷰
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();

    if(CurrentView == nView)				// 현재 뷰와 생성할 뷰가 같으면 아무것도 하지 않음
        return;

	delete pNewActiveView;

	switch (nView) {
	case MENU:
		pNewActiveView = new CGameView;		// 메뉴 뷰 생성
		break;

	case GAME:
		CInGameView* pGameView = (CInGameView*) pFrame->GetActiveView();
		pGameView->CloseWindow();			// 메뉴 뷰 종료
		pNewActiveView = new CInGameView;	// 게임 뷰 생성
		break;
	}

	CCreateContext context;
	context.m_pCurrentDoc = pOldActiveView->GetDocument();
	pNewActiveView->Create (NULL , NULL , 
		WS_BORDER | WS_CHILD , CFrameWnd::rectDefault , 
		this, nView , &context);

	pNewActiveView -> OnInitialUpdate();

    SetActiveView ( pNewActiveView);
    pNewActiveView->ShowWindow(SW_SHOW);
    pOldActiveView->ShowWindow(SW_HIDE);

    if(pOldActiveView->GetRuntimeClass() == RUNTIME_CLASS(CGameView) )
        pOldActiveView->SetDlgCtrlID(MENU);

    else if(pOldActiveView->GetRuntimeClass() == RUNTIME_CLASS(CInGameView) )
        pOldActiveView->SetDlgCtrlID(GAME);
		
    pNewActiveView->SetDlgCtrlID(AFX_IDW_PANE_FIRST);

    RecalcLayout(); 
    CurrentView = nView; // ÇöÀç ºä¸¦ ¼³Á¤ÇÑ ºä·Î ´ëÀÔ½ÃÅ²´Ù.
 }