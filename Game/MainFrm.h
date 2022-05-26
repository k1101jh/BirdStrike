
// MainFrm.h : CMainFrame

#pragma once
#include "Game.h"
#include "GameDoc.h"
#include "StartMenuScene.h"
#include "InGameScene.h"
#include "SceneID.h"


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
	void SwitchView(SceneID id);

protected:
	DECLARE_MESSAGE_MAP()

private:
	CView* start_menu_scene_;
	CView* in_game_scene_;
};


