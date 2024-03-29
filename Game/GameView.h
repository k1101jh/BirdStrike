
// GameView.h : CGameView Å¬·¡½ºÀÇ ÀÎÅÍÆäÀÌ½º
//

#pragma once
#include "afxwin.h"
//#include "player.h"


class CGameView : public CView
{
public:
CGameView();

protected: // serialization¿¡¼­¸¸ ¸¸µé¾îÁý´Ï´Ù.
	DECLARE_DYNCREATE(CGameView)

// Æ¯¼ºÀÔ´Ï´Ù.
public:
	CGameDoc* GetDocument() const;

// ÀÛ¾÷ÀÔ´Ï´Ù.
public:

// ÀçÁ¤ÀÇÀÔ´Ï´Ù.
public:
	virtual void OnDraw(CDC* pDC);  // ÀÌ ºä¸¦ ±×¸®±â À§ÇØ ÀçÁ¤ÀÇµÇ¾ú½À´Ï´Ù.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ±¸ÇöÀÔ´Ï´Ù.
public:
	virtual ~CGameView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// »ý¼ºµÈ ¸Þ½ÃÁö ¸Ê ÇÔ¼ö
protected:
	DECLARE_MESSAGE_MAP()
public:
	int m_intScore;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int AniIndex;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CBitmap m_bmpGameStart;
	CBitmap m_bmpQuitGame;
	virtual void OnInitialUpdate();
	CBitmap m_bmpMain;
};

#ifndef _DEBUG  // GameView.cppÀÇ µð¹ö±× ¹öÀü
inline CGameDoc* CGameView::GetDocument() const
   { return reinterpret_cast<CGameDoc*>(m_pDocument); }
#endif

