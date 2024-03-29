// InGameView.cpp : ±¸Çö ÆÄÀÏÀÔ´Ï´Ù.
//

#include "stdafx.h"
#include "Game.h"
#include "InGameView.h"
#include "MainFrm.h"
#include "GameDoc.h"
#include "Ground.h"
#include <vector>

#define WND_X 1200//1024
#define WND_Y 800//720

#define ONE_BLOCK 1240
#define TWO_BLOCK 2000

#define JUMP_MODE 1
#define RUN_MODE 0

// CInGameView

IMPLEMENT_DYNCREATE(CInGameView, CView)

CInGameView::CInGameView()
: m_nX(0)
, m_nY(0)
, m_prevVelocity(0)
, m_Velocity(0)
, m_maxX(0)
, m_bOnGround(false)
, m_bGameOver(false)
, m_bJump(true)
, m_nScore(0)
, m_nJumpChance(0)
, m_bShoot(false)
, m_bShootAni(false)
, m_bRaceMode(false)
, m_nTransMode(0)
, m_nCoinX(0)
, m_nCoinY(0)
, m_bAttacked(false)
, m_nLife(0)
{
	m_nMapMove = 0;
}

CInGameView::~CInGameView()
{
}

BEGIN_MESSAGE_MAP(CInGameView, CView)
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CInGameView

void CInGameView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:

	static int AniIndex = 0;
	static DWORD dwTime = GetTickCount();	// 애니메이션 전환을 위한 카운터
	DWORD dwCurTime = GetTickCount();		// 애니메이션 전환을 위한 카운터
	static int MaxAniIndex = 2;				// 애니메이션 구분을 위한 변수

	CDC MemDC, BufferDC;

	MemDC.CreateCompatibleDC(pDC);			// 오브젝트 선택을 위한 DC
	BufferDC.CreateCompatibleDC(pDC);		// 더블 버퍼링을 통한 화면 출력을 위한 DC

	CBitmap bmpBuffer;
	CBitmap *pOldBitmap;
	bmpBuffer.CreateCompatibleBitmap(pDC, WND_X, WND_Y);
	pOldBitmap = BufferDC.SelectObject(&bmpBuffer);

	if(!m_bmpBackground.m_hObject)
		m_bmpBackground.LoadBitmap(IDB_MAP);
	if(!m_bmpBird.m_hObject)
		m_bmpBird.LoadBitmap(IDB_BIRD);
	if(!m_bmpGround.m_hObject)
		m_bmpGround.LoadBitmap(IDB_GROUND);
	if(!m_bmpBullet.m_hObject)
		m_bmpBullet.LoadBitmap(IDB_BULLET);
	if(!m_bmpCoin.m_hObject)
		m_bmpCoin.LoadBitmap(IDB_COIN);
	if(!m_bmpWall.m_hObject)
		m_bmpWall.LoadBitmap(IDB_WALL);


	
	MemDC.SelectObject(&m_bmpBackground);
	BufferDC.BitBlt(0, 0, WND_X*2, WND_Y, &MemDC, m_nMapMove, 0, SRCCOPY);			// 버퍼 DC에 배경 출력
	BufferDC.BitBlt(WND_X*2-m_nMapMove, 0, WND_X*2, WND_Y, &MemDC, 0, 0, SRCCOPY);	// 배경이 끊기지 않기 위해 앞에 출력한 배경에 이어서 또 다른 배경 출력

	MemDC.SelectObject(&m_bmpBird);

	static int prevMaxAniInd;

	if(m_bShootAni == true)
	{
		prevMaxAniInd = MaxAniIndex + 1;
		MaxAniIndex = 4;
	}
	else if(m_bOnGround == false)
	{
		prevMaxAniInd = MaxAniIndex;
		MaxAniIndex = 2;
	}
	else
	{
		prevMaxAniInd = MaxAniIndex;
		MaxAniIndex = 4;
	}

	MemDC.SelectObject(&m_bmpWall);
	BufferDC.TransparentBlt(0, 0, 97, 640, &MemDC, 0, 0, 97, 640, RGB(255, 0, 255));

	if(dwTime < dwCurTime)
	{
		dwTime = dwCurTime + 100;
		AniIndex++;

		if(AniIndex >= MaxAniIndex)
		{
			AniIndex = 0;
		}
	}
	if(prevMaxAniInd != MaxAniIndex)
		if(AniIndex >= MaxAniIndex)
		{
			AniIndex = 0;
		}
	
	MemDC.SelectObject(&m_bmpBird);
	if(m_bShootAni == true)
	{
		BufferDC.TransparentBlt(m_nX, m_nY, 100, 100, &MemDC, 100 * (int)(AniIndex/2), 200, 100, 100, RGB(255, 0, 255));
		if(AniIndex == 3)
			m_bShootAni = false;
	}
	else if(m_bAttacked == true)
	{
		BufferDC.TransparentBlt(m_nX, m_nY, 100, 100, &MemDC, 0, 300, 100, 100, RGB(255, 0, 255));
	}
	else if(m_bOnGround == false)
	{
		BufferDC.TransparentBlt(m_nX, m_nY, 100, 100, &MemDC, 100 * AniIndex, 100, 100, 100, RGB(255, 0, 255));
	}
	else
	{
		BufferDC.TransparentBlt(m_nX, m_nY, 100, 100, &MemDC, 100 * AniIndex, 0, 100, 100, RGB(255, 0, 255));
	}

	MemDC.SelectObject(&m_bmpGround);
	for(int i = 0; i < m_vGrounds.size(); i++)
	{
		BufferDC.TransparentBlt(m_vGrounds.at(i).getRect().left, m_vGrounds.at(i).getRect().top, 100, 100, &MemDC, 0, 0, 100, 100, RGB(255, 0, 255));
	}

	MemDC.SelectObject(&m_bmpCoin);
	for(int i = 0; i < m_vCoins.size(); i++)
	{
		BufferDC.TransparentBlt(m_vCoins.at(i).getRect().left, m_vCoins.at(i).getRect().top, 100, 100, &MemDC, 0, 0, 100, 100, RGB(255, 0, 255));
	}

	MemDC.SelectObject(&m_bmpEnemy);
	for(int i = 0; i < m_vEnemys.size(); i++)
	{
		BufferDC.TransparentBlt(m_vEnemys.at(i).getRect().left, m_vEnemys.at(i).getRect().top, 150, 97, &MemDC, 0, 0, 150, 97, RGB(255, 0, 255));
	}

	MemDC.SelectObject(&m_bmpBullet);
	if(m_bShoot)
		BufferDC.TransparentBlt(m_bullet->getPoint().x, m_bullet->getPoint().y, 20, 20, &MemDC, 0, 0, 20, 20, RGB(255, 0, 255));

	CString score;
	//score.Format(_T("SCORE : %d"), m_nScore);
	score.Format(_T("SCORE : %d"), m_nScore);

	CRect rect;
	GetClientRect(&rect);
	rect.top += 20;
	BufferDC.DrawText(score, rect, DT_SINGLELINE | DT_CENTER | DT_TOP);

	pDC->BitBlt(0, 0, WND_X, WND_Y, &BufferDC, 0, 0, SRCCOPY);
	
	MemDC.DeleteDC();
	BufferDC.SelectObject(pOldBitmap);
	BufferDC.DeleteDC();
	bmpBuffer.DeleteObject();
}


// CInGameView Áø´ÜÀÔ´Ï´Ù.

#ifdef _DEBUG
void CInGameView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CInGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CInGameView ¸Þ½ÃÁö Ã³¸®±âÀÔ´Ï´Ù.


void CInGameView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ??? ??? ??? ??? ?? ?/?? ???? ?????.
	switch(nChar)
	{
	case VK_SPACE:
		if(m_nJumpChance > 0)			// 점프 가능 횟수가 0번 이상일 떄(최대 2번)
		{
			m_bOnGround = false;		// 땅 위에 존재하지 않음
			m_bJump = true;				// 점프 중
			m_prevVelocity = 0;			// 이전 속도 = 0
			m_Velocity = 30;			// 속도를 30으로 설정
			if(m_nJumpChance == 2)		// Ground에 존재할 때 점프했을 시
				m_nY -= 21;				// 플레이어의 y좌표를 21만큼 위로 올림
			m_nJumpChance--;			// 점프 가능 횟수 감소
		}
		break;
	}
	

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

int CInGameView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ??? ???? ?? ??? ?????.

	return 0;
}


void CInGameView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ??? ???? ??? ?? ?/?? ?? ???? ?????.
	SetTimer(0, 15, NULL);
	m_nX = 200;
	m_nY = 400;
	m_nJumpChance = 2;
	m_maxX = 200;
	m_prevVelocity = 0;
	m_Velocity = 0;
	m_bmpBird.LoadBitmap(IDB_BIRD);
	m_bmpBackground.LoadBitmap(IDB_MAP);
	m_bmpGround.LoadBitmap(IDB_GROUND);
	m_bmpBullet.LoadBitmap(IDB_BULLET);
	m_bmpEnemy.LoadBitmap(IDB_ENEMY);
	m_bmpCoin.LoadBitmap(IDB_COIN);
	m_bmpWall.LoadBitmap(IDB_WALL);
	m_nLife = 3;

	for(int i = 0; i < 10; i++)
	{
		m_vGrounds.push_back(Ground(i*100));
	}
	//CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	//pFrame->ShowWindow(SW_SHOWMAXIMIZED);

}


void CInGameView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ??? ??? ??? ??? ?? ?/?? ???? ?????.

	static DWORD dwTime = GetTickCount();
	DWORD dwCurTime = GetTickCount();
	static DWORD dwTime2 = GetTickCount();
	DWORD dwCurTime2 = GetTickCount();

	if(dwTime < dwCurTime)
	{
		if(m_bRaceMode==JUMP_MODE)
		{
			int nJump=0, nDistance=0;
			int nBlock = rand()%5;

			if(rand()%5==0||rand()%5==4) nJump = TWO_BLOCK;
			else nJump = ONE_BLOCK;

			for(int i=0;i<=nBlock;i++)
			{
				m_vGrounds.push_back(Ground(1200+100*i));
				if(nBlock<4)
					m_vCoins.push_back(Coin(1200+100*i,500));
				else if(nBlock==4)
				{
					m_vCoins.push_back(Coin(1200,500));
					m_vCoins.push_back(Coin(1300,400));
					m_vCoins.push_back(Coin(1400,300));
					m_vCoins.push_back(Coin(1500,400));
					m_vCoins.push_back(Coin(1600,500));
				}
			}

			nDistance=(nBlock+1)*400+nJump;

			dwTime = dwCurTime + nDistance;

			if(nJump==TWO_BLOCK)
			{
				m_nCoinY = 50;
			}
			else if(nJump==ONE_BLOCK)
			{
				m_nCoinY = 300;
			}

			int n;
			if(nJump==ONE_BLOCK)
				n=100;
			else if(nJump==TWO_BLOCK)
				n=200;
			m_nCoinX = 1200+100*(nBlock+1)+n;

			m_vCoins.push_back(Coin(m_nCoinX, m_nCoinY));
		}
		else if(m_bRaceMode==RUN_MODE)
		{
			m_vGrounds.push_back(Ground(1200));
			dwTime = dwCurTime + 380;

			if(m_nTransMode<=8)
			{
				int n=rand()%5;
				if(n==0)
					m_nCoinY = 50;
				else if(n==4)
					m_nCoinY = 300;
				else
					m_nCoinY = 400;
			}
			else
			{
				m_nCoinY = 500;
			}
			m_vCoins.push_back(Coin(m_nCoinY));
		}
		TransMode();
	}

	if(dwTime2 < dwCurTime2)
	{
		dwTime2 = dwCurTime2 + 1000;
		m_vEnemys.push_back(Enemy());
	}
	
	m_nMapMove++;
//	m_nScore++;
	if(nIDEvent == 0)
	{
		if(m_nMapMove >= 2400)
			m_nMapMove = 0;
	}

	for(int i = 0; i < m_vGrounds.size(); i++)
	{
		m_vGrounds.at(i).move();
	}

	for(int i = 0; i < m_vEnemys.size(); i++)
	{
		m_vEnemys.at(i).move();
	}

	for(int i = 0; i < m_vCoins.size(); i++)
	{
		m_vCoins.at(i).move();
	}

	
	/*
	if(m_nX <= m_maxX) // ³ªÁß¿¡ BOOL º¯¼ö Ãß°¡ÇØ¼­ º®°ú Ãæµ¹ÁßÀÌ ¾Æ´Ò ¶§¸¸
	{
		m_nX += 1;
	}
	*
	/*
	if(m_nY <= 0)
	{
		m_nY = 0;
		//m_prevVelocity = 0;
		m_Velocity -= 0.5;
	}
	*/


	if(m_nY >= 700)
		m_bGameOver = true;
	else
		m_bGameOver = false;

	CRect pRect = new CRect(m_nX, m_nY, m_nX + 100, m_nY + 100);
	CRect rct;

	Ground g;
	bool initCollided = true;
	if(!m_vGrounds.empty())
	{
		for(int i = 0; i < m_vGrounds.size(); i++)
		{
			g = m_vGrounds.at(i);
			if(g.getX() <= -100)
			{
				vector<Ground>::iterator it;
				it = m_vGrounds.begin();
				for(int j = 0; j < i; j++)
					it++;
				m_vGrounds.erase(it);
				i--;
				
				if(m_vGrounds.empty())
					break;
			}

			if(pRect.right - 20 >= g.getRect().left && pRect.left + 30 <= g.getRect().right &&
				pRect.bottom <= g.getRect().top && pRect.bottom >= g.getRect().top - 20)
			//if(g.isCollider(pRect))
			{
				m_bOnGround = true;
				m_nY = g.getRect().top - 100;
				m_nJumpChance = 2;
				m_Velocity = 0;
				break;
			}
			else
			{
				m_bOnGround = false;
				//Gravity();
				if(pRect.bottom >= 622)
					m_nJumpChance = 0;
			}
		}
	}
	
	Enemy enemy;
	if(!m_vEnemys.empty())
	{
		for(int i = 0; i < m_vEnemys.size(); i++)
		{
			enemy = m_vEnemys.at(i);
			
			if(enemy.getX() <= -150)
			{
				vector<Enemy>::iterator it;
				it = m_vEnemys.begin();
				for(int j = 0; j < i; j++)
					it++;
				m_vEnemys.erase(it);
				i--;
				
				if(m_vEnemys.empty())
					break;
			}
			
			if(pRect.right - 25 >= enemy.getRect().left && pRect.left + 10 <= enemy.getRect().right &&
				pRect.bottom - 10 >= enemy.getRect().top && pRect.top + 20 <= enemy.getRect().bottom)
			{	// 플레이어와의 충돌
				m_bAttacked = true;
				vector<Enemy>::iterator it;
					it = m_vEnemys.begin();
					for(int j = 0; j < i; j++)
						it++;
					m_vEnemys.erase(it);	// 적 제거
					i--;
				if(m_vEnemys.empty())		// 적이 하나도 없을 시 반복문 탈출
					break;
			}
			else if(m_bShoot)
				if(m_bullet->getPoint().x + 20 >= enemy.getRect().left && m_bullet->getPoint().x <= enemy.getRect().right &&
					m_bullet->getPoint().y + 20 >= enemy.getRect().top && m_bullet->getPoint().y <= enemy.getRect().bottom)
				{	// 총알과의 충돌
					vector<Enemy>::iterator it;
					it = m_vEnemys.begin();
					for(int j = 0; j < i; j++)
						it++;
					m_vEnemys.erase(it);	// 적 제거
					i--;
				
					delete m_bullet;		// 총알 제거
					m_bShoot = false;		// 총알이 없는 상태로 설정
					m_nScore += 100;		// 점수 추가

					if(m_vEnemys.empty())	// 적이 하나도 없을 시 반복문 탈출
						break;
				}
		}
	}
	
	Coin coin;
	if(!m_vCoins.empty())
	{
		for(int i = 0; i < m_vCoins.size(); i++)
		{
			coin = m_vCoins.at(i);
			if(coin.getX() <= -100)
			{
				vector<Coin>::iterator it;
				it = m_vCoins.begin();
				for(int j = 0; j < i; j++)
					it++;
				m_vCoins.erase(it);
				i--;
				
				if(m_vCoins.empty())
					break;
			}

			if(pRect.right - 25 >= coin.getRect().left && pRect.left + 10 <= coin.getRect().right &&
				pRect.bottom - 10 >= coin.getRect().top && pRect.top + 30 <= coin.getRect().bottom)
			//if(g.isCollider(pRect))
			{
				m_nScore+=500;
				vector<Coin>::iterator it;
					it = m_vCoins.begin();
					for(int j = 0; j < i; j++)
						it++;
					m_vCoins.erase(it);
					i--;
					break;
			}
		}
	}

	if(m_bShoot)
	{
		m_bullet->Move();
		if(m_bullet->getPoint().x > WND_X + 20 || m_bullet->getPoint().x < -20 || m_bullet->getPoint().y > WND_Y + 20 || m_bullet->getPoint().y < -20)
		{
			m_bShoot = false;
			delete m_bullet;
		}
	}
	static int knock=0;
	
	if(m_bAttacked)
	{
		if(knock!=25)
		{
			m_nX-=2;
			knock++;
		}
		else
		{
			m_bAttacked=false;
			knock=0;
			m_nLife--;
		}
	}
	if(m_nLife==0)
		m_bGameOver = true;
	
	if(m_bGameOver == true)
	{
		KillTimer(0);														// 타이머 정지
		CString gameoverMessage;
		gameoverMessage.Format(_T("GAME OVER\n SCORE : %d"), m_nScore);
		if(MessageBox(gameoverMessage, MB_OK) == IDOK)						// 게임 오버 메시지 출력
		{
			m_dlgScoreBoard.Create(IDD_SCOREBOARD, this);					// 점수표 출력

			CRect rectMain, rectBoard;
			GetWindowRect(&rectMain);

			m_dlgScoreBoard.GetWindowRect(&rectBoard);
			m_dlgScoreBoard.MoveWindow(
				rectMain.left + (rectMain.Width()-rectBoard.Width())/2,
				rectMain.top + (rectMain.Height()-rectBoard.Height())/2,
				rectBoard.Width(), rectBoard.Height());

			m_dlgScoreBoard.ShowWindow(SW_SHOW);
		}

	}


	Gravity(); // ³ªÁß¿¡ BOOL º¯¼ö Ãß°¡ÇØ¼­ ¹Ù´Ú°ú Ãæµ¹ÁßÀÌ ¾Æ´Ò ¶§¸¸

	Invalidate();

	CView::OnTimer(nIDEvent);
}


void CInGameView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: ??? ??? ??? ??? ?????.
	KillTimer(0);
}



void CInGameView::EraseBitMap(void)
{
	CClientDC dc(this);

	CDC MemDC;
	CBitmap bmpBack;

	if(!MemDC.CreateCompatibleDC(&dc)) return;
	if(!bmpBack.LoadBitmapW(IDB_MAP)) return;

	MemDC.SelectObject(&bmpBack);
	dc.BitBlt(m_nX, m_nY, 100, 100, &MemDC, m_nX, m_nY, SRCCOPY);
}


void CInGameView::Gravity()
{	
	static bool falling = false;
	if(!m_bOnGround)
	{
		if(falling == false)	// 공중에 있지 않을 시
			m_Velocity = 0;		// 속도를 0으로 설정
		m_Velocity -= 0.8;		// 속도는 0.8씩 감소
		m_nY = m_nY - (((m_prevVelocity + m_Velocity)*0.4)/2);	// 플레이어의 Y좌표에 계산된 속도를 뺌
		m_prevVelocity = m_Velocity;							// 이전 속도에 현재 속도 저장
		falling = true;			// 공중에 있지 않은 상태로 정의
	}

}

BOOL CInGameView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ¿©±â¿¡ ¸Þ½ÃÁö Ã³¸®±â ÄÚµå¸¦ Ãß°¡ ¹×/¶Ç´Â ±âº»°ªÀ» È£ÃâÇÕ´Ï´Ù.

	//return CView::OnEraseBkgnd(pDC);

	return TRUE;
}


void CInGameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:

	
	if(m_bShoot == false)
	{
		m_bullet = new Bullet(CPoint(m_nX + 65, m_nY + 25), point);	// 총알 객체 생성(생성 좌표, 목적지 좌표)
		m_bShoot = true;
		m_bShootAni = true;
	}

	CView::OnLButtonDown(nFlags, point);
}


void CInGameView::InitializeSetting(void)
{
	m_nX = 200;
	m_nY = 400;
	m_bOnGround = false;
	m_bGameOver = false;
	m_bJump = true;
	m_nScore = 0;
	m_nJumpChance = 0;
	m_nMapMove = 0;
	m_Velocity = 0;
	m_bShoot = false;
	m_bShootAni = false;
	
	/*
	vector<Enemy>::iterator it;
	vector<Ground>::iterator it2;
	
	it = m_vEnemys.begin();
	it2 = m_vGrounds.begin();

	for(it = m_vEnemys.begin(); it != m_vEnemys.end();)
	{
		if(m_vEnemys.size() > 0)
			it = m_vEnemys.erase(it);
		else
			it++;
	}*/

	/*
	for(int i = 0; i < m_vEnemys.size();)
	{
		it = m_vEnemys.erase(it);
	}
	for(int i = 0; i < m_vGrounds.size();)
	{	
		it2 = m_vGrounds.erase(it2);
	}
	*/
	/*
	while(m_vEnemys.size() > 0)
		it = m_vEnemys.erase(it);
	while(m_vGrounds.size() > 0)
		it2 = m_vGrounds.erase(it2);
		*/
	m_vEnemys.clear();
	m_vGrounds.clear();

	//delete m_bullet;
/*
	for(int i = 0; i < 10; i++)
	{
		m_vGrounds.push_back(Ground(i*100));
	}
	*/
	SetTimer(0, 15, NULL);

}


CString CInGameView::GetScore(void)
{
	CString score;
	score.Format(_T("%d"), m_nScore);
	return score;
}


void CInGameView::TransMode(void)
{
	m_nTransMode++;
	if(m_nTransMode>=10)
	{
		if(rand()%5==0||rand()%5==4)
		{
			m_nTransMode-=10;
		}
		else
		{
			if(m_bRaceMode==RUN_MODE)
				m_bRaceMode=JUMP_MODE;
			else 
				m_bRaceMode=RUN_MODE;
			m_nTransMode=0;
		}
	}
}
