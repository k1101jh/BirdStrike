#pragma once
#include "afxcmn.h"
#include "dataset.h"


// CScoreBoard 대화 상자입니다.

class CScoreBoard : public CDialogEx
{
	DECLARE_DYNAMIC(CScoreBoard)

public:
	CScoreBoard(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CScoreBoard();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SCOREBOARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButtonEnroll();
	afx_msg void OnClickedButtonMain();
	void UpdateScoreBoard(void);
	virtual BOOL OnInitDialog();
	CString m_strName;
	CString m_strScore;
	CListCtrl m_listScore;
	DbCommand m_command;
	CDataSet m_dbSet;
};
