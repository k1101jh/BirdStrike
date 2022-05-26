#include "stdafx.h"
#include "aInGameScene.h"

#define ONE_BLOCK 1240
#define TWO_BLOCK 2000

#define JUMP_MODE 1
#define RUN_MODE 0

// CInGameScene

IMPLEMENT_DYNCREATE(CInGameScene, CView)

CInGameScene::CInGameScene()
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

CInGameScene::~CInGameScene()
{
}

BEGIN_MESSAGE_MAP(CInGameScene, CView)
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CInGameScene

void CInGameScene::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:

	static int AniIndex = 0;
	static ULONGLONG dwTime = GetTickCount64();	// 애니메이션 전환을 위한 카운터
	ULONGLONG dwCurTime = GetTickCount64();		// 애니메이션 전환을 위한 카운터
	static int MaxAniIndex = 2;				// 애니메이션 구분을 위한 변수

	CDC BufferDC;

	BufferDC.CreateCompatibleDC(pDC);		// 더블 버퍼링을 통한 화면 출력을 위한 DC

	CBitmap bmpBuffer;
	CBitmap *pOldBitmap;
	bmpBuffer.CreateCompatibleBitmap(pDC, WND_X, WND_Y);
	pOldBitmap = BufferDC.SelectObject(&bmpBuffer);

	//BufferDC.BitBlt(0, 0, WND_X*2, WND_Y, &MemDC, m_nMapMove, 0, SRCCOPY);			// 버퍼 DC에 배경 출력
	//BufferDC.BitBlt(WND_X*2-m_nMapMove, 0, WND_X*2, WND_Y, &MemDC, 0, 0, SRCCOPY);	// 배경이 끊기지 않기 위해 앞에 출력한 배경에 이어서 또 다른 배경 출력
	
	// 게임 오브젝트의 sprite를 가져옴
	/*
	for (int i = 0; i < gameObjects.size(); i++) {
		Sprite* sprite = gameObjects[i].GetComponent<Sprite>();
		int id = (int)sprite->GetLayerID();
		sprites[id].push_back(sprite);
	}
	*/

	// 그리기
	DrawGameObjects(&BufferDC);

	// 점수 출력
	CString score;
	score.Format(_T("SCORE : %d"), m_nScore);

	CRect rect;
	GetClientRect(&rect);
	rect.top += 20;
	BufferDC.DrawText(score, rect, DT_SINGLELINE | DT_CENTER | DT_TOP);


	// 이중 버퍼링
	pDC->BitBlt(0, 0, WND_X, WND_Y, &BufferDC, 0, 0, SRCCOPY);
	
	BufferDC.SelectObject(pOldBitmap);
	BufferDC.DeleteDC();
	bmpBuffer.DeleteObject();
}

void CInGameScene::DrawGameObjects(CDC* DC) {
	CDC memDC;
	memDC.CreateCompatibleDC(DC);
	/*
	for (int i = 0; i < (int)LayerID::SIZE; i++) {
		for (int j = 0; j < sprites[i].size(); j++) {
			Sprite* sprite = sprites[i][j];
			Transform* transform = sprite->GetTransform();
			memDC.SelectObject(&sprite);
			DC->TransparentBlt(transform->GetPosition().x, transform->GetPosition().x,
				transform->GetScale().x, transform->GetScale().y,
				&memDC, 0, 0, transform->GetScale().x, transform->GetScale().y, RGB(255, 0, 0));
		}
	}
	*/
	memDC.DeleteDC();
}

void CInGameScene::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar) {
	case VK_SPACE:
		if (m_nJumpChance > 0)			// 점프 가능 횟수가 0번 이상일 떄(최대 2번)
		{
			m_bOnGround = false;		// 땅 위에 존재하지 않음
			m_bJump = true;				// 점프 중
			m_prevVelocity = 0;			// 이전 속도 = 0
			m_Velocity = 30;			// 속도를 30으로 설정
			if (m_nJumpChance == 2)		// Ground에 존재할 때 점프했을 시
				m_nY -= 21;				// 플레이어의 y좌표를 21만큼 위로 올림
			m_nJumpChance--;			// 점프 가능 횟수 감소
		}
		break;
	}


	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

#ifdef _DEBUG
void CInGameScene::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CInGameScene::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

int CInGameScene::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}


void CInGameScene::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	SetTimer(0, 15, NULL);
	m_nX = 200;
	m_nY = 400;
	m_nJumpChance = 2;
	m_maxX = 200;
	m_prevVelocity = 0;
	m_Velocity = 0;
	m_bmpBird.LoadBitmap(IDB_BIRD);
	m_bmpBackground.LoadBitmap(IDB_MAP);
	m_bmpGround.LoadBitmap(IDB_GROUND_SINGLE);
	m_bmpBullet.LoadBitmap(IDB_BULLET);
	m_bmpEnemy.LoadBitmap(IDB_ENEMY);
	m_bmpCoin.LoadBitmap(IDB_COIN);
	m_bmpWall.LoadBitmap(IDB_WALL);
	m_nLife = 3;
	/*
	for(int i = 0; i < 10; i++)
	{
		m_vGrounds.push_back(Ground(200, i*100, 100, 100));
	}
	*/
}


void CInGameScene::OnTimer(UINT_PTR nIDEvent)
{
	// 오브젝트들을 업데이트
	for (GameObject& gameObject : gameObjects) {
		gameObject.Update();
	}


	/*

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

	*/
	
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


	Gravity();

	Invalidate();

	CView::OnTimer(nIDEvent);
}


void CInGameScene::OnDestroy()
{
	CView::OnDestroy();
	KillTimer(0);
}



void CInGameScene::EraseBitMap(void)
{
	CClientDC dc(this);

	CDC MemDC;
	CBitmap bmpBack;

	if(!MemDC.CreateCompatibleDC(&dc)) return;
	if(!bmpBack.LoadBitmapW(IDB_MAP)) return;

	MemDC.SelectObject(&bmpBack);
	dc.BitBlt(m_nX, m_nY, 100, 100, &MemDC, m_nX, m_nY, SRCCOPY);
}


void CInGameScene::Gravity()
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

BOOL CInGameScene::OnEraseBkgnd(CDC* pDC)
{
	//return CView::OnEraseBkgnd(pDC);

	return TRUE;
}


void CInGameScene::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_bShoot == false)
	{
		m_bullet = new Bullet(CPoint(m_nX + 65, m_nY + 25), point);	// 총알 객체 생성(생성 좌표, 목적지 좌표)
		m_bShoot = true;
		m_bShootAni = true;
	}

	CView::OnLButtonDown(nFlags, point);
}


void CInGameScene::InitializeSetting(void)
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


CString CInGameScene::GetScore(void)
{
	CString score;
	score.Format(_T("%d"), m_nScore);
	return score;
}


void CInGameScene::TransMode(void)
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
