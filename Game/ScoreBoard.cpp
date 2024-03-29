// ScoreBoard.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game.h"
#include "ScoreBoard.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "InGameView.h"


// CScoreBoard 대화 상자입니다.

IMPLEMENT_DYNAMIC(CScoreBoard, CDialogEx)

CScoreBoard::CScoreBoard(CWnd* pParent /*=NULL*/)
	: CDialogEx(CScoreBoard::IDD, pParent)
	, m_strName(_T(""))
	, m_strScore(_T(""))
{

}

CScoreBoard::~CScoreBoard()
{
}

void CScoreBoard::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_SCORE, m_strScore);
	DDX_Control(pDX, IDC_LIST_BOARD, m_listScore);
}


BEGIN_MESSAGE_MAP(CScoreBoard, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ENROLL, &CScoreBoard::OnClickedButtonEnroll)
	ON_BN_CLICKED(IDC_BUTTON_MAIN, &CScoreBoard::OnClickedButtonMain)
END_MESSAGE_MAP()


// CScoreBoard 메시지 처리기입니다.


void CScoreBoard::OnClickedButtonEnroll()
{
	// TODO: 
	UpdateData(TRUE);
	CString strQuery;
	strQuery.Format(_T("INSERT INTO scoreboard(username, score) VALUES('%s', '%s');"),
		m_strName, m_strScore);
	m_command.Close();
	if(!m_strName.IsEmpty())
	{
		if(m_dbSet.Query(strQuery, &m_command))
		{
			AfxMessageBox(_T("등록되었습니다."));
			(GetDlgItem(IDC_BUTTON_ENROLL))->EnableWindow(FALSE);
			UpdateScoreBoard();	// 스코어보드 업데이트
		}
	}
	else
	{
		MessageBox(_T("유저명을 입력해 주세요."), _T("잠깐"), MB_OK);
	}
}


void CScoreBoard::OnClickedButtonMain()
{
	// TODO:
	this->DestroyWindow();
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->SwitchView(pFrame->MENU);
}


void CScoreBoard::UpdateScoreBoard(void)
{
	int nCount = 0;

	CString strQuery;
	strQuery.Format(_T("SELECT * FROM scoreboard ORDER BY score DESC")); // 점수를 기준으로 내림차순 하여 불러오기
	m_command.Close();

	int i = 0;
	CString strRank;
	if(m_dbSet.Query(strQuery, &m_command))
	{
		m_listScore.DeleteAllItems();
		while(m_command.MoveNext() == S_OK)
		{
			strRank.Format(_T("%d"), i + 1);
			m_listScore.InsertItem(nCount, m_command.GetString(2));
			m_listScore.SetItem(nCount, 1, LVIF_TEXT, m_command.GetString(3), 0, 0, 0, 0);
			m_listScore.SetItem(nCount, 2, LVIF_TEXT, strRank, 0, 0, 0, 0);
			i++;

			nCount++;
			UpdateData(FALSE);
		}
	}
}


BOOL CScoreBoard::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ??? ?? ??? ??? ?????.
	LPWSTR list[3] = {_T("NAME"), _T("SCORE"), _T("RANK")};
	int nWidth[4] = {100, 200, 100};

	for(int i = 0; i < 3; i++)
	{
		m_listScore.InsertColumn(i, list[i], LVCFMT_CENTER, nWidth[i]);
	}

	m_listScore.SetExtendedStyle(m_listScore.GetExtendedStyle() |
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	CMainFrame *pFrame = (CMainFrame*) AfxGetMainWnd();
	CInGameView* pGameView = (CInGameView*) pFrame->GetActiveView();

	m_strScore = pGameView->GetScore();

	UpdateScoreBoard();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ??: OCX ?? ???? FALSE? ???? ???.
}
