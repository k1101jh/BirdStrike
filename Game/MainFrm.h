
// MainFrm.h : CMainFrame

#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // serialization
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	short CurrentView;
	enum eView { MENU = 0 , GAME = 1};
	void SwitchView(eView);

protected:
	DECLARE_MESSAGE_MAP()

};


