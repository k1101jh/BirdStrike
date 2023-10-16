#include "stdafx.h"
#include "StartMenuScene.h"


// StartMenuScene

IMPLEMENT_DYNCREATE(StartMenuScene, CView)

BEGIN_MESSAGE_MAP(StartMenuScene, CView)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &StartMenuScene::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

StartMenuScene::StartMenuScene()
{
}

StartMenuScene::~StartMenuScene()
{
}

BOOL StartMenuScene::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~(LONG)FWS_ADDTOTITLE;

	return CView::PreCreateWindow(cs);


}

void StartMenuScene::OnDraw(CDC* pDC)
{
	CGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CDC MemDC;

	MemDC.CreateCompatibleDC(pDC);

	MemDC.SelectObject(bmpGameStart);
	pDC->BitBlt(WND_X / 2 - 150, WND_Y / 2, WND_X / 2 + 150, WND_Y / 2 + 100, &MemDC, 0, 0, SRCCOPY);

	
	MemDC.SelectObject(bmpQuitGame);
	pDC->BitBlt(WND_X / 2 - 150, WND_Y / 2 + 150, WND_X / 2 + 150, WND_Y / 2 + 250, &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(bmpMain);
	pDC->BitBlt(0, 0, 1200, 400, &MemDC, 0, 0, SRCCOPY);
}

BOOL StartMenuScene::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void StartMenuScene::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void StartMenuScene::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

#ifdef _DEBUG
void StartMenuScene::AssertValid() const
{
	CView::AssertValid();
}

void StartMenuScene::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGameDoc* StartMenuScene::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGameDoc)));
	return (CGameDoc*)m_pDocument;
}
#endif //_DEBUG

void StartMenuScene::OnLButtonDown(UINT nFlags, CPoint point)
{
	CView::OnLButtonDown(nFlags, point);

	CRect gsBTRect(WND_X / 2 - 150, WND_Y / 2, WND_X / 2 + 150, WND_Y / 2 + 100);
	CRect qgBTRect(WND_X / 2 - 150, WND_Y / 2 + 150, WND_X / 2 + 150, WND_Y / 2 + 250);

	// 게임 시작
	if(point.x > gsBTRect.left && point.x < gsBTRect.right && point.y < gsBTRect.bottom && point.y > gsBTRect.top)
	{
		CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
		pFrame->SwitchView(SceneID::GAME);
	}
	// 게임 종료
	else if(point.x > qgBTRect.left && point.x < qgBTRect.right && point.y < qgBTRect.bottom && point.y > qgBTRect.top)
	{
		CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
		StartMenuScene* pGameScene = (StartMenuScene*) pFrame->GetActiveView();
		pGameScene->CloseWindow();

		CWinApp* App = AfxGetApp();
		ASSERT(App->m_pMainWnd!=NULL);
		App->m_pMainWnd->SendMessage(WM_CLOSE);
	}
}


void StartMenuScene::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	bmpGameStart.LoadBitmap(IDB_GAMESTART);
	bmpQuitGame.LoadBitmap(IDB_QUITGAME);
	bmpMain.LoadBitmap(IDB_MAIN);

	GetParent()->SetWindowTextW(_T("Bird Strike"));
}
