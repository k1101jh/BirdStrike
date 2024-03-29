
// GameView.cpp : CGameView Å¬·¡½ºÀÇ ±¸Çö
//

#include "stdafx.h"
// SHARED_HANDLERS´Â ¹Ì¸® º¸±â, Ãà¼ÒÆÇ ±×¸² ¹× °Ë»ö ÇÊÅÍ Ã³¸®±â¸¦ ±¸ÇöÇÏ´Â ATL ÇÁ·ÎÁ§Æ®¿¡¼­ Á¤ÀÇÇÒ ¼ö ÀÖÀ¸¸ç
// ÇØ´ç ÇÁ·ÎÁ§Æ®¿Í ¹®¼­ ÄÚµå¸¦ °øÀ¯ÇÏµµ·Ï ÇØ ÁÝ´Ï´Ù.
#ifndef SHARED_HANDLERS
#include "Game.h"
#endif

#include "GameDoc.h"
#include "GameView.h"
#include "GameDoc.h"
#include "MainFrm.h"
#include "InGameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WND_X 1200
#define WND_Y 800


// CGameView

IMPLEMENT_DYNCREATE(CGameView, CView)

BEGIN_MESSAGE_MAP(CGameView, CView)
	// Ç¥ÁØ ÀÎ¼â ¸í·ÉÀÔ´Ï´Ù.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CGameView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CGameView »ý¼º/¼Ò¸ê

CGameView::CGameView()
{
	// TODO: ¿©±â¿¡ »ý¼º ÄÚµå¸¦ Ãß°¡ÇÕ´Ï´Ù.

}

CGameView::~CGameView()
{
}

BOOL CGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs¸¦ ¼öÁ¤ÇÏ¿© ¿©±â¿¡¼­
	//  Window Å¬·¡½º ¶Ç´Â ½ºÅ¸ÀÏÀ» ¼öÁ¤ÇÕ´Ï´Ù
	cs.style &= ~(LONG)FWS_ADDTOTITLE;

	return CView::PreCreateWindow(cs);


}

// CGameView ±×¸®±â

void CGameView::OnDraw(CDC* pDC)
{
	CGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ¿©±â¿¡ ¿ø½Ã µ¥ÀÌÅÍ¿¡ ´ëÇÑ ±×¸®±â ÄÚµå¸¦ Ãß°¡ÇÕ´Ï´Ù.

	CDC MemDC;

	MemDC.CreateCompatibleDC(pDC);

	MemDC.SelectObject(m_bmpGameStart);
	pDC->BitBlt(WND_X / 2 - 150, WND_Y / 2, WND_X / 2 + 150, WND_Y / 2 + 100, &MemDC, 0, 0, SRCCOPY);

	
	MemDC.SelectObject(m_bmpQuitGame);
	pDC->BitBlt(WND_X / 2 - 150, WND_Y / 2 + 150, WND_X / 2 + 150, WND_Y / 2 + 250, &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(m_bmpMain);
	pDC->BitBlt(0, 0, 1200, 400, &MemDC, 0, 0, SRCCOPY);
}


// CGameView ÀÎ¼â

BOOL CGameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// ±âº»ÀûÀÎ ÁØºñ
	return DoPreparePrinting(pInfo);
}

void CGameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ÀÎ¼âÇÏ±â Àü¿¡ Ãß°¡ ÃÊ±âÈ­ ÀÛ¾÷À» Ãß°¡ÇÕ´Ï´Ù.
}

void CGameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ÀÎ¼â ÈÄ Á¤¸® ÀÛ¾÷À» Ãß°¡ÇÕ´Ï´Ù.
}

// CGameView Áø´Ü

#ifdef _DEBUG
void CGameView::AssertValid() const
{
	CView::AssertValid();
}

void CGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGameDoc* CGameView::GetDocument() const // µð¹ö±×µÇÁö ¾ÊÀº ¹öÀüÀº ÀÎ¶óÀÎÀ¸·Î ÁöÁ¤µË´Ï´Ù.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGameDoc)));
	return (CGameDoc*)m_pDocument;
}
#endif //_DEBUG


// CGameView ¸Þ½ÃÁö Ã³¸®±â


void CGameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CView::OnLButtonDown(nFlags, point);

	// TODO: ¿©±â¿¡ ¸Þ½ÃÁö Ã³¸®±â ÄÚµå¸¦ Ãß°¡ ¹×/¶Ç´Â ±âº»°ªÀ» È£ÃâÇÕ´Ï´Ù.
	CRect gsBTRect(WND_X / 2 - 150, WND_Y / 2, WND_X / 2 + 150, WND_Y / 2 + 100);
	CRect qgBTRect(WND_X / 2 - 150, WND_Y / 2 + 150, WND_X / 2 + 150, WND_Y / 2 + 250);

	// 게임 시작
	if(point.x > gsBTRect.left && point.x < gsBTRect.right && point.y < gsBTRect.bottom && point.y > gsBTRect.top)
	{
		CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
		pFrame->SwitchView(pFrame->GAME);
	}
	// 게임 종료
	else if(point.x > qgBTRect.left && point.x < qgBTRect.right && point.y < qgBTRect.bottom && point.y > qgBTRect.top)
	{
		CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
		CGameView* pGameView = (CGameView*) pFrame->GetActiveView();
		pGameView->CloseWindow();

		CWinApp* App = AfxGetApp();
		ASSERT(App->m_pMainWnd!=NULL);
		App->m_pMainWnd->SendMessage(WM_CLOSE);
	}
}


void CGameView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ??? ???? ??? ?? ?/?? ?? ???? ?????.

	m_bmpGameStart.LoadBitmap(IDB_GAMESTART);
	m_bmpQuitGame.LoadBitmap(IDB_QUITGAME);
	m_bmpMain.LoadBitmap(IDB_MAIN);

	GetParent()->SetWindowTextW(_T("Bird Strike"));


}
