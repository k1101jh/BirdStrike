#pragma once
#include <map>
#include <vector>

#include "afxwin.h"
#include "GameObject.h"
#include "Ground.h"
#include "bullet.h"
#include "Enemy.h"
#include "Coin.h"
#include "Sprite.h"
#include "scoreboard.h"
#include "Game.h"
#include "MainFrm.h"
#include "GameDoc.h"

// CInGameScene

class CInGameScene : public CView
{
	DECLARE_DYNCREATE(CInGameScene)

public:
	CInGameScene();

protected:
	virtual ~CInGameScene();

public:
	virtual void OnDraw(CDC* pDC);
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
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	void DrawGameObjects(CDC* DC);
	CString GetScore(void);
private:
	std::vector<GameObject> gameObjects;
	std::map<LayerID, Sprite*> sprites;


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
	
	CBitmap m_bmpGround;
	bool m_bJump;
	std::vector<Ground> m_vGrounds;
	std::vector<Enemy> m_vEnemys;
	std::vector<Coin> m_vCoins;
	int m_nScore;
	int m_nJumpChance;
	bool m_bShoot;
	
	Bullet *m_bullet;
	CBitmap m_bmpBullet;
	CBitmap m_bmpEnemy;
	bool m_bShootAni;
	void InitializeSetting(void);
	bool m_bRaceMode;
	int m_nTransMode;
	CScoreBoard m_dlgScoreBoard;
	
	int m_nCoinX;
	int m_nCoinY;
	CBitmap m_bmpCoin;
	void TransMode(void);
	CBitmap m_bmpWall;
	bool m_bAttacked;
	int m_nLife;
};


