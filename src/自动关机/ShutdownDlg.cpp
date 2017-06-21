
// ShutdownDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "自动关机.h"
#include "ShutdownDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSysteminfo();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_BN_CLICKED(IDC_SystemInfo, &CAboutDlg::OnBnClickedSysteminfo)
END_MESSAGE_MAP()


// CShutdownDlg 对话框




CShutdownDlg::CShutdownDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShutdownDlg::IDD, pParent)
	, m_Bool(true)
	, m_LongTime(0)
	, m_TomorrowBool(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShutdownDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Timing, m_Timing);
	DDX_Control(pDX, IDC_EDIT, m_Second);
	DDX_Control(pDX, IDC_CurrentDate, m_CurrentDate);
	DDX_Control(pDX, IDC_EDITTime, m_CEdit);
	DDX_Control(pDX, IDC_EDITBack, m_BackTime);
	DDX_Control(pDX, IDC_About, m_btnSet);
}

BEGIN_MESSAGE_MAP(CShutdownDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_About, &CShutdownDlg::OnBnClickedAbout)
	//	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_Timing, &CShutdownDlg::OnDtnDatetimechangeTiming)
	ON_BN_CLICKED(IDC_xTiming, &CShutdownDlg::OnBnClickedxtiming)
	ON_BN_CLICKED(IDC_xSecond, &CShutdownDlg::OnBnClickedxsecond)
	ON_BN_CLICKED(IDC_Start, &CShutdownDlg::OnBnClickedStart)
	ON_WM_TIMER()
//	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_Timing, &CShutdownDlg::OnDtnDatetimechangeTiming)
//	ON_EN_CHANGE(IDC_EDIT, &CShutdownDlg::OnEnChangeEdit)
ON_BN_CLICKED(IDC_Tomorrow, &CShutdownDlg::OnBnClickedTomorrow)
ON_BN_CLICKED(IDC_Cancel, &CShutdownDlg::OnBnClickedCancel)
//ON_BN_CLICKED(IDC_Hide, &CShutdownDlg::OnBnClickedHide)
ON_BN_CLICKED(IDC_Hide, &CShutdownDlg::OnBnClickedHide)
END_MESSAGE_MAP()


// CShutdownDlg 消息处理程序

BOOL CShutdownDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//对话框初始化，设置定时器1启动，启动右侧时间显示
	SetTimer(Timer1,1000,0);
	m_BackTime.SetWindowText(_T("未设置"));
	m_btnSet.SetCheck(true);

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CShutdownDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CShutdownDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CShutdownDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CShutdownDlg::OnBnClickedAbout()
{
	// TODO: 在此添加控件通知处理程序代码
	CAboutDlg about;
	about.DoModal();
}

void CShutdownDlg::OnBnClickedxtiming()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Bool = true;
}

void CShutdownDlg::OnBnClickedxsecond()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Bool = false;
}

void CShutdownDlg::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_Bool)
	{
		SYSTEMTIME stSet;
		SYSTEMTIME stCurrent;
		GetLocalTime(&stCurrent);
		m_Timing.GetTime(&stSet);
		int stHour = stSet.wHour - stCurrent.wHour;
		int stMinute = stSet.wMinute - stCurrent.wMinute;
		int stSecond = stSet.wSecond - stCurrent.wSecond;
		if (m_TomorrowBool)
		{
			stHour = stHour + 24;
		}
		int sec = (stHour * 3600 + stMinute * 60 + stSecond) * 1000;
		m_LongTime = sec/1000;
		if (sec <= 0)
		{
			MessageBox(_T("设置的关机时间已经过去了！"),_T("告诉你"));
			return;
		}
		//启动定时器2，设置在sec毫秒后系统关机
		SetTimer(Timer2,sec,0);
		//启动定时器3，设置倒计时启动
		SetTimer(Timer3,1000,0);
	}
	else
	{
		CString str;
		m_Second.GetWindowText(str);
		if (str == "")
		{
			MessageBox(_T("你要输入延迟关机的分钟才行！"),_T("告诉你"));
			return;
		}
		long sec = StrToInt(str) * 60000;
		m_LongTime = sec/1000;
		//启动定时器2，设置在sec毫秒后系统关机
		SetTimer(Timer2,sec,0);
		//启动定时器3，设置倒计时启动
		SetTimer(Timer3,1000,0);
	}
}

BOOL CShutdownDlg::WinShutDown(UINT uFags)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken);
	LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid);
	tkp.PrivilegeCount=1;
	tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
	//取得进程特权
	AdjustTokenPrivileges(hToken,FALSE,&tkp,0,(PTOKEN_PRIVILEGES)NULL,0);
	return ExitWindowsEx(uFags,NULL);
}


void CShutdownDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString str;
	SYSTEMTIME st;
	CString lSeccond;
	switch (nIDEvent)
	{
	//实现右侧显示系统时间（日期是静态的）
	case Timer1: 
		GetLocalTime(&st);
		str.Format(L"%d:%d:%d",st.wHour,st.wMinute,st.wSecond);
		m_CEdit.SetWindowText(str);
		break;
	//实现定时关机
	case Timer2:
		WinShutDown(EWX_SHUTDOWN);
		break;
	//实现倒计时提示
	case Timer3:
		lSeccond.Format(L"%ld",--m_LongTime);
		m_BackTime.SetWindowText(lSeccond);
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}


void CShutdownDlg::OnBnClickedTomorrow()
{
	// TODO: 在此添加控件通知处理程序代码
	m_TomorrowBool = !m_TomorrowBool;
}

void CShutdownDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(Timer3);
	KillTimer(Timer2);
	m_BackTime.SetWindowText(_T("未设置"));
}

void CAboutDlg::OnBnClickedSysteminfo()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(_T("暂未实现功能!"),_T("温馨提示"));
}

void CShutdownDlg::OnBnClickedHide()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(_T("暂未实现功能!"),_T("温馨提示"));
}
