#pragma once
#include "afxwin.h"
#include "Ground.h"
#include <vector>
#include "bullet.h"
#include "Enemy.h"
#include "Coin.h"
#include "scoreboard.h"
using std::vector;
using namespace std;


// CInGameView ºäÀÔ´Ï´Ù.

class CInGameView : public CView
{
	DECLARE_DYNCREATE(CInGameView)

public:
	CInGameView();           // µ¿Àû ¸¸µé±â¿¡ »ç¿ëµÇ´Â protected »ý¼ºÀÚÀÔ´Ï´Ù.

protected:
	virtual ~CInGameView();

public:
	virtual void OnDraw(CDC* pDC);      // ÀÌ ºä¸¦ ±×¸®±â À§ÇØ ÀçÁ¤ÀÇµÇ¾ú½À´Ï´Ù.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	int m_nX;
	int m_nY;
	int m_nMapMove;
	void EraseBitMap(void);
	double m_prevVelocity;
	double m_Velocity;
	void Gravity();
	void DrawBird(void);
	void DrawBackground(void);
	void DrawGround(void);
	int m_maxX;
	bool m_bOnGround;
	bool m_bGameOver;
	CBitmap m_bmpBird;
	CBitmap m_bmpBackground;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CBitmap m_bmpGround;
	bool m_bJump;
	vector<Ground> m_vGrounds;
	vector<Enemy> m_vEnemys;
	vector<Coin> m_vCoins;
	int m_nScore;
	int m_nJumpChance;
	bool m_bShoot;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	Bullet *m_bullet;
	CBitmap m_bmpBullet;
	CBitmap m_bmpEnemy;
	bool m_bShootAni;
	void InitializeSetting(void);
	bool m_bRaceMode;
	int m_nTransMode;
	CScoreBoard m_dlgScoreBoard;
	CString GetScore(void);
	int m_nCoinX;
	int m_nCoinY;
	CBitmap m_bmpCoin;
	void TransMode(void);
	CBitmap m_bmpWall;
	bool m_bAttacked;
	int m_nLife;
};


