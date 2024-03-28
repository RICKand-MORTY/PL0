
// PL0_MFCDlg.cpp: 实现文件
//
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "framework.h"
#include "PL0_MFC.h"
#include "PL0_MFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include"PL0.h"


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPL0MFCDlg 对话框



CPL0MFCDlg::CPL0MFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PL0_MFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPL0MFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPL0MFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPL0MFCDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPL0MFCDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CPL0MFCDlg 消息处理程序

BOOL CPL0MFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CFont m_font;

	m_font.CreatePointFont(180, _T("Arial"));

	CRichEditCtrl* pRichEdit = (CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT21);
	CRichEditCtrl* pEdit = (CRichEditCtrl*)GetDlgItem(IDC_EDIT1);

	pRichEdit->SetFont(&m_font);
	pEdit->SetFont(&m_font);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPL0MFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPL0MFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPL0MFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPL0MFCDlg::OnBnClickedButton1()
{
	for (CH = ' '; CH <= '^'; CH++) SSYM[CH] = NUL;
	strcpy(KWORD[1], "BEGIN");    strcpy(KWORD[2], "CALL");
	strcpy(KWORD[3], "CONST");    strcpy(KWORD[4], "DO");
	strcpy(KWORD[5], "END");      strcpy(KWORD[6], "IF");
	strcpy(KWORD[7], "ODD");      strcpy(KWORD[8], "PROCEDURE");
	strcpy(KWORD[9], "PROGRAM");  strcpy(KWORD[10], "READ");
	strcpy(KWORD[11], "THEN");     strcpy(KWORD[12], "VAR");
	strcpy(KWORD[13], "WHILE");    strcpy(KWORD[14], "WRITE");
	WSYM[1] = BEGINSYM;   WSYM[2] = CALLSYM;
	WSYM[3] = CONSTSYM;   WSYM[4] = DOSYM;
	WSYM[5] = ENDSYM;     WSYM[6] = IFSYM;
	WSYM[7] = ODDSYM;     WSYM[8] = PROCSYM;
	WSYM[9] = PROGSYM;    WSYM[10] = READSYM;
	WSYM[11] = THENSYM;    WSYM[12] = VARSYM;
	WSYM[13] = WHILESYM;   WSYM[14] = WRITESYM;
	SSYM['+'] = PLUS;      SSYM['-'] = MINUS;
	SSYM['*'] = TIMES;     SSYM['/'] = SLASH;
	SSYM['('] = LPAREN;    SSYM[')'] = RPAREN;
	SSYM['='] = EQL;       SSYM[','] = COMMA;
	SSYM['.'] = PERIOD;    SSYM['#'] = NEQ;
	SSYM[';'] = SEMICOLON;
	strcpy(MNEMONIC[LIT], "LIT");   strcpy(MNEMONIC[OPR], "OPR");
	strcpy(MNEMONIC[LOD], "LOD");   strcpy(MNEMONIC[STO], "STO");
	strcpy(MNEMONIC[CALL], "CAL");   strcpy(MNEMONIC[INI], "INI");
	strcpy(MNEMONIC[JMP], "JMP");   strcpy(MNEMONIC[JPC], "JPC");

	DECLBEGSYS = (int*)malloc(sizeof(int) * 33);
	STATBEGSYS = (int*)malloc(sizeof(int) * 33);
	FACBEGSYS = (int*)malloc(sizeof(int) * 33);
	for (int j = 0; j < 33; j++) {
		DECLBEGSYS[j] = 0;  STATBEGSYS[j] = 0;  FACBEGSYS[j] = 0;
	}
	DECLBEGSYS[CONSTSYM] = 1;
	DECLBEGSYS[VARSYM] = 1;
	DECLBEGSYS[PROCSYM] = 1;
	STATBEGSYS[BEGINSYM] = 1;
	STATBEGSYS[CALLSYM] = 1;
	STATBEGSYS[IFSYM] = 1;
	STATBEGSYS[WHILESYM] = 1;
	STATBEGSYS[WRITESYM] = 1;
	FACBEGSYS[IDENT] = 1;
	FACBEGSYS[NUMBER] = 1;
	FACBEGSYS[LPAREN] = 1;
	CString input;
	GetDlgItemText(IDC_EDIT1, input);
	if (((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() == BST_CHECKED)
	{
		List_all = TRUE;
	}
	else
	{
		List_all = FALSE;
	}
	if ((FIN = _wfopen(((input + _T(".PL0")).GetString()), _T("r"))) != 0) {
		FOUT = _wfopen(((input + _T(".COD")).GetString()), _T("w"));
		AppendTextToRichEdit(IDC_RICHEDIT21,"=== COMPILE PL0 ===\n");
		fwprintf(FOUT, L"=== COMPILE PL0 ===\n");
		ERR = 0;
		CC = 0; CX = 0; LL = 0; CH = ' '; GetSym();
		if (SYM != PROGSYM) Error(0);
		else {
			GetSym();
			if (SYM != IDENT) Error(0);
			else {
				GetSym();
				if (SYM != SEMICOLON) Error(5);
				else GetSym();
			}
		}
		Block(0, 0, SymSetAdd(PERIOD, SymSetUnion(DECLBEGSYS, STATBEGSYS)));
		if (SYM != PERIOD) Error(9);
		if (ERR == 0) Interpret();
		else {
			AppendTextToRichEdit(IDC_RICHEDIT21, "ERROR IN PL/0 PROGRAM\n");
			fprintf(FOUT, "ERROR IN PL/0 PROGRAM\n");
		}
		fprintf(FOUT, "\n"); fclose(FOUT);
	}
}


void CPL0MFCDlg::OnBnClickedButton2()
{
	GetDlgItem(IDC_RICHEDIT21)->SetWindowText(_T(""));
}
