
// AutoShutdownDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoShutdown.h"
#include "AutoShutdownDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
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
	CEdit m_HopeText;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_HOPETEXT, m_HopeText);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CAutoShutdownDlg 对话框




CAutoShutdownDlg::CAutoShutdownDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoShutdownDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Status = false;
	m_Mode = 0;
	m_bFront = false;
	m_bIsSound = false;
	m_bCustomSound = false;
	m_bIsDlgNormal = true;
	m_bIsStartUp = false;
	strSoundPath = _T("");
	m_bSelect = true;
}

void CAutoShutdownDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_COUNTDOWN, m_CountDown);
	DDX_Control(pDX, ID_SELECTTIMING, m_SelectTiming);
	DDX_Control(pDX, ID_TIMINGTIME, m_TimingTime);
	DDX_Control(pDX, ID_CURRENTTIME, m_CurrentTime);
	DDX_Control(pDX, ID_TIMINGMINUTE, m_TimingMinute);
	DDX_Control(pDX, ID_TIMINGDATE, m_TimingDate);
	DDX_Control(pDX, ID_CURRENTDATE, m_CurrentDate);
	DDX_Control(pDX, ID_CUSTOM, m_Custom);
}

BEGIN_MESSAGE_MAP(CAutoShutdownDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_ABOUT, &CAutoShutdownDlg::OnAbout)
	ON_COMMAND(ID_QUIT, &CAutoShutdownDlg::OnQuit)
	ON_COMMAND(ID_AUTHOR, &CAutoShutdownDlg::OnAuthor)
	ON_COMMAND(ID_SELECTTIMING, &CAutoShutdownDlg::OnSelecttiming)
	ON_COMMAND(ID_SELECTMINUTE, &CAutoShutdownDlg::OnSelectminute)
	ON_BN_CLICKED(ID_START, &CAutoShutdownDlg::OnBnClickedStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_STOP, &CAutoShutdownDlg::OnBnClickedStop)
	ON_COMMAND(ID_MINIDLG, &CAutoShutdownDlg::OnMinidlg)
	ON_COMMAND(ID_SHUTDOWN, &CAutoShutdownDlg::OnShutdown)
	ON_COMMAND(ID_REBOOT, &CAutoShutdownDlg::OnReboot)
	ON_COMMAND(ID_LOGOUT, &CAutoShutdownDlg::OnLogout)
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(ID_CLOCK, &CAutoShutdownDlg::OnClock)
	ON_UPDATE_COMMAND_UI(ID_CLOCK, &CAutoShutdownDlg::OnUpdateClock)
	ON_UPDATE_COMMAND_UI(ID_SHUTDOWN, &CAutoShutdownDlg::OnUpdateShutdown)
	ON_UPDATE_COMMAND_UI(ID_REBOOT, &CAutoShutdownDlg::OnUpdateReboot)
	ON_UPDATE_COMMAND_UI(ID_LOGOUT, &CAutoShutdownDlg::OnUpdateLogout)
	ON_UPDATE_COMMAND_UI(ID_START, &CAutoShutdownDlg::OnUpdateStart)
	ON_WM_DESTROY()
	ON_COMMAND(ID_TRAYDLG, &CAutoShutdownDlg::OnTraydlg)
	ON_WM_CLOSE()
	ON_COMMAND(ID_MAINAPP, &CAutoShutdownDlg::OnMainapp)
	ON_COMMAND(ID_FRONTDLG, &CAutoShutdownDlg::OnFrontdlg)
	ON_UPDATE_COMMAND_UI(ID_FRONTDLG, &CAutoShutdownDlg::OnUpdateFrontdlg)
	ON_COMMAND(ID_STARTUP, &CAutoShutdownDlg::OnStartup)
	ON_UPDATE_COMMAND_UI(ID_STARTUP, &CAutoShutdownDlg::OnUpdateStartup)
	ON_WM_QUERYENDSESSION()
END_MESSAGE_MAP()

//关机所使用的函数
BOOL CAutoShutdownDlg::WinShutDown(UINT uFags)
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
// CAutoShutdownDlg 消息处理程序

BOOL CAutoShutdownDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem(ID_SEPARATE)->GetWindowRect(&rectSeparate);
	GetWindowRect(&rectLarge);
	rectSmall.left = rectLarge.left;
	rectSmall.top = rectLarge.top;
	rectSmall.bottom = rectLarge.bottom;
	rectSmall.right = rectSeparate.right;

	//隐藏部分窗体
	SetWindowPos(NULL,0,0,rectSmall.Width(),rectSmall.Height(),SWP_NOMOVE | SWP_NOZORDER);

	SetTimer(CurrentTimer,100,0);
	m_CountDown.SetWindowTextW(_T("未设置"));
	m_SelectTiming.SetCheck(TRUE);

	strTips = _T("自动关机");
	ntfId.cbSize = sizeof(NOTIFYICONDATA);
	ntfId.hWnd = m_hWnd;
	ntfId.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	ntfId.uCallbackMessage = WM_NOTIFYICON;
	ntfId.uID            = 99;      // Per WinCE SDK docs, values from 0 to 12 are reserved and should not be used.
	ntfId.hIcon          = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	wcscpy_s(ntfId.szTip,128,strTips);
	//添加托盘图标
	::Shell_NotifyIcon(NIM_ADD,&ntfId);

	//此处有BUG,自定义音乐的路径没有正确存储，跟踪执行open.lpstrElementName = strSoundPath;，
	//发现open.lpstrElementName是错误指针，这也应该是自定音乐不能整播放的原因
	//个人能力有限，希望大神纠正  我的邮箱 pprain_yue@sina.com  非常感谢
	GetModuleFileName(NULL,strSoundPath.GetBufferSetLength(MAX_PATH + 1),MAX_PATH);
	strSoundPath.ReleaseBuffer();
	int nPos;
	nPos = strSoundPath.ReverseFind('\\');
	strSoundPath = strSoundPath.Left(nPos);
	strSoundPath = strSoundPath + L"\\i.mp3";
	open.lpstrElementName = strSoundPath;
	open.lpstrDeviceType = L"mpegvideo";

	//启动时取消自启动
	
	RegOpenKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",&hKey);
	if(ERROR_SUCCESS == (RegQueryValueEx(hKey,L"AutoShutDown2",0,NULL,NULL,NULL)))
		::RegDeleteValue(hKey,L"AutoShutDown2");
	RegCloseKey(hKey);

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

void CAutoShutdownDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAutoShutdownDlg::OnPaint()
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
HCURSOR CAutoShutdownDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAutoShutdownDlg::OnAbout()
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CAutoShutdownDlg::OnQuit()
{
	// TODO: 在此添加命令处理程序代码
	m_Status = false;
	KillTimer(ShutDownTimer);
	KillTimer(CountDownTimer);
	DestroyWindow();
}


void CAutoShutdownDlg::OnAuthor()
{
	// TODO: 在此添加命令处理程序代码
	if (IDOK == ::MessageBox(NULL,_T("   Thank You！\n   ┏ (゜ω゜)=☞"),_T("XiaoY:"),MB_OKCANCEL | MB_ICONASTERISK))
	{
		m_bIsDlgNormal = !m_bIsDlgNormal;
		if (m_bIsDlgNormal)
		{
			SetWindowPos(NULL,0,0,rectSmall.Width(),rectSmall.Height(),SWP_NOMOVE | SWP_NOZORDER);
		} 
		else
		{
			SetWindowPos(NULL,0,0,rectLarge.Width(),rectLarge.Height(),SWP_NOMOVE | SWP_NOZORDER);
		}
	}
	else
	{
		SetWindowPos(NULL,0,0,rectSmall.Width(),rectSmall.Height(),SWP_NOMOVE | SWP_NOZORDER);
	}
}


void CAutoShutdownDlg::OnSelecttiming()
{
	// TODO: 在此添加命令处理程序代码
	m_bSelect = true;
}


void CAutoShutdownDlg::OnSelectminute()
{
	// TODO: 在此添加命令处理程序代码
	m_bSelect = false;
}


void CAutoShutdownDlg::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bIsSound)
	{
		if (m_bCustomSound)
		{
			//关闭自定义音乐
			mciSendCommand(DeviceID,MCI_STOP,0,0);
			mciSendCommand(DeviceID,MCI_CLOSE,0,0);
		} 
		else
		{
			//关闭内置音乐
			PlaySound(NULL,NULL,SND_PURGE);
		}
		m_bIsSound = false;
	}
	if (m_bSelect)
	{
		SYSTEMTIME stTimingTime;
		SYSTEMTIME stCurrentTime;
		SYSTEMTIME stTimingDate;
		LONGLONG Msec;

		GetLocalTime(&stCurrentTime);  //获取当前时间
		m_TimingTime.GetTime(&stTimingTime);  //获取设置时间
		m_TimingDate.GetTime(&stTimingDate);  //获取设置日期

		//将设置的日期添加到设置的时间结构中
		stTimingTime.wYear = stTimingDate.wYear;
		stTimingTime.wMonth = stTimingDate.wMonth;
		stTimingTime.wDay = stTimingDate.wDay;

		CTime CurrentTime(stCurrentTime);
		CTime TimingTime(stTimingTime);
		//算出时间差
		CTimeSpan SpanTime = TimingTime - CurrentTime;

		m_LongTime = SpanTime.GetTotalSeconds();
		Msec = m_LongTime * 1000;

		if (Msec <= 0)
		{
			m_Status = false;
			KillTimer(ShutDownTimer);
			KillTimer(CountDownTimer);
			m_CountDown.SetWindowTextW(L"未设置");
			MessageBox(_T("设置的定时时间已经过去了！"),_T("告诉你"),MB_OK | MB_ICONWARNING);
			return;
		}
		m_Status = true;
		//启动定时器2，设置在sec毫秒后系统关机
		SetTimer(ShutDownTimer,Msec,0);
		//启动定时器3，设置倒计时启动
		SetTimer(CountDownTimer,1000,0);
	}
	else
	{
		CString str;
		if (!(m_TimingMinute.GetWindowTextLengthW()))
		{
			m_Status = false;
			KillTimer(ShutDownTimer);
			KillTimer(CountDownTimer);
			m_CountDown.SetWindowTextW(L"未设置");
			MessageBox(_T("你要输入延迟的分钟数才行！"),_T("告诉你"),MB_OK | MB_ICONWARNING);
			return;
		}
		m_TimingMinute.GetWindowTextW(str);
		unsigned long sec = StrToLong(str) * 60000;
		m_LongTime = sec/1000;
		m_Status = true;
		//启动定时器2，设置在sec毫秒后系统关机
		SetTimer(ShutDownTimer,sec,0);
		//启动定时器3，设置倒计时启动
		SetTimer(CountDownTimer,1000,0);
	}
}


void CAutoShutdownDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString str;
	SYSTEMTIME st;
	CString lSeccond;
	switch (nIDEvent)
	{
		//实现右侧显示系统时间（日期是静态的）
	case CurrentTimer: 
		GetLocalTime(&st);
		str.Format(L"%d:%d:%d",st.wHour,st.wMinute,st.wSecond);
		m_CurrentTime.SetWindowText(str);
		str.Format(L"%d/ %d/ %d",st.wYear,st.wMonth,st.wDay);
		m_CurrentDate.SetWindowText(str);
		break;
		//实现定时关机
	case ShutDownTimer:
		switch(m_Mode)
		{
		case 0:
			WinShutDown(EWX_SHUTDOWN);
			break;
		case 1:
			WinShutDown(EWX_REBOOT);
			break;
		case 2:
			WinShutDown(EWX_LOGOFF);
			break;
		case 3:
			m_Status = false;
			KillTimer(ShutDownTimer);
			KillTimer(CountDownTimer);
			m_CountDown.SetWindowTextW(L"未设置");
			m_bCustomSound = (BST_CHECKED == m_Custom.GetCheck());
			if (!m_bCustomSound)
			{
				//内置音乐
				PlaySound(MAKEINTRESOURCE(ID_SOUND),AfxGetResourceHandle(),\
					SND_ASYNC | SND_RESOURCE | SND_NODEFAULT | SND_LOOP);
			} 
			else
			{
				//自定义音乐
				MCI_PLAY_PARMS play;
				play.dwFrom = 0;
				play.dwCallback = (DWORD)m_hWnd;
				m_bCustomSound = true;
				mciSendCommand(DeviceID,MCI_CLOSE,0,0);
				mciSendCommand(0,MCI_OPEN,MCI_OPEN_ELEMENT | MCI_WAIT,(DWORD)(LPVOID)&open);
				DeviceID = open.wDeviceID;
				if(0 != mciSendCommand(DeviceID,MCI_PLAY,MCI_FROM | MCI_NOTIFY,(DWORD)&play))
				{
					m_bCustomSound = false;
					PlaySound(MAKEINTRESOURCE(ID_SOUND),AfxGetResourceHandle(),\
						SND_ASYNC | SND_RESOURCE | SND_NODEFAULT | SND_LOOP);
					::MessageBox(NULL,L"请谅解。\n\r\n未能播放自定义音乐，已转为内置音乐，请检查程序旁边是否有命名为 i.mp3 的音乐。",\
						L"告诉你：",MB_OK | MB_ICONQUESTION);
				}
			}
			m_bIsSound = true;
			break;
		}
		break;
		//实现倒计时提示
	case CountDownTimer:
		lSeccond.Format(L"%ld",--m_LongTime);
		m_CountDown.SetWindowText(lSeccond);
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CAutoShutdownDlg::OnBnClickedStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Status = false;
	KillTimer(ShutDownTimer);
	KillTimer(CountDownTimer);
	m_CountDown.SetWindowTextW(L"未设置");
	if (m_bIsSound)
	{
		if (m_bCustomSound)
		{
			//关闭自定义音乐
			mciSendCommand(DeviceID,MCI_STOP,0,0);
			mciSendCommand(DeviceID,MCI_CLOSE,0,0);
		} 
		else
		{
			//关闭内置音乐
			PlaySound(NULL,NULL,SND_PURGE);
		}
		m_bIsSound = false;
	}
	strTips = L"自动关机";
	ntfId.uFlags = NIF_TIP;
	wcscpy_s(ntfId.szTip,128,strTips);
	::Shell_NotifyIcon(NIM_MODIFY,&ntfId);
}


void CAutoShutdownDlg::OnMinidlg()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_MINIMIZE);
}

//设置到达指定时间所进行的动作
void CAutoShutdownDlg::OnShutdown()
{
	// TODO: 在此添加命令处理程序代码
	m_Mode = 0;
}


void CAutoShutdownDlg::OnReboot()
{
	// TODO: 在此添加命令处理程序代码
	m_Mode = 1;
}


void CAutoShutdownDlg::OnLogout()
{
	// TODO: 在此添加命令处理程序代码
	m_Mode = 2;
}


void CAutoShutdownDlg::OnClock()
{
	// TODO: 在此添加命令处理程序代码
	m_Mode = 3;
}

//菜单标记
void CAutoShutdownDlg::OnUpdateShutdown(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_Mode == 0);
}


void CAutoShutdownDlg::OnUpdateReboot(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_Mode == 1);
}


void CAutoShutdownDlg::OnUpdateLogout(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_Mode == 2);
}


void CAutoShutdownDlg::OnUpdateClock(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_Mode == 3);
}

//此消息响应是复制CFrameWnd：：OnInitMenuPopup中的，如果没有此消息响应，则菜单标记无法变化。来自
void CAutoShutdownDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialogEx::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	// TODO: 在此处添加消息处理程序代码
	ASSERT(pPopupMenu != NULL);
	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// determine if menu is popup in top-level menu and set m_pOther to
	//  it if so (m_pParentMenu == NULL indicates that it is secondary popup)
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
		state.m_pParentMenu = pPopupMenu;    // parent == child for tracking popup
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = this;
		// child windows don't have menus -- need to go to the top!
		if (pParent != NULL && 
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nItemIndex = 0; nItemIndex < nIndexMax; nItemIndex++)
			{
				if (::GetSubMenu(hParentMenu, nItemIndex) == pPopupMenu->m_hMenu)
				{
					// when popup is found, m_pParentMenu is containing menu
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // menu separator or invalid cmd - ignore it

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// possibly a popup menu, route to first item of that popup
			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // first item of popup can't be routed to
			}
			state.DoUpdate(this, TRUE);    // popups are never auto disabled
		}
		else
		{
			// normal menu item
			// Auto enable/disable if frame window has 'm_bAutoMenuEnable'
			//    set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this,FALSE);
		}

		// adjust for menu deletions and additions
		UINT nCount = pPopupMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}



void CAutoShutdownDlg::OnUpdateStart(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_Status);
}


void CAutoShutdownDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	::Shell_NotifyIcon(NIM_DELETE,&ntfId);
}


LRESULT CAutoShutdownDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	CPoint pt;
	switch (message)
	{
	//托盘图标响应
	case WM_NOTIFYICON:
		switch(lParam)
		{
		case WM_LBUTTONDOWN:
			SetForegroundWindow();
			ShowWindow(SW_SHOWNORMAL);
			break;
		case WM_RBUTTONDOWN:
			menu.LoadMenu(IDR_NOTIFYMENU);
			ntfMenu = menu.GetSubMenu(0);
			GetCursorPos(&pt);
			SetForegroundWindow();
			ntfMenu->TrackPopupMenu(TPM_BOTTOMALIGN,pt.x,pt.y,this);
			break;
		case WM_MOUSEMOVE:
			if(m_Status)
			{
				switch(m_Mode)
				{
				case 0:
					strTips.Format(L"%ds 后关机",m_LongTime);
					break;
				case 1:
					strTips.Format(L"%ds 后重启",m_LongTime);
					break;
				case 2:
					strTips.Format(L"%ds 后注销",m_LongTime);
					break;
				case 3:
					strTips.Format(L"%ds 后响铃",m_LongTime);
					break;
				}
				ntfId.uFlags = NIF_TIP;
				wcscpy_s(ntfId.szTip,128,strTips);
				::Shell_NotifyIcon(NIM_MODIFY,&ntfId);
			}
			break;
		}
		break;
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CAutoShutdownDlg::OnTraydlg()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_HIDE);
}


void CAutoShutdownDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ShowWindow(SW_HIDE);
}


void CAutoShutdownDlg::OnMainapp()
{
	// TODO: 在此添加命令处理程序代码
	SetForegroundWindow();
	ShowWindow(SW_SHOWNORMAL);
}


void CAutoShutdownDlg::OnFrontdlg()
{
	// TODO: 在此添加命令处理程序代码
	m_bFront = !m_bFront;
	if (m_bFront)
	{
		SetWindowPos(&this->wndTopMost,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);
	} 
	else
	{
		SetWindowPos(&this->wndNoTopMost,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);
	}
}


void CAutoShutdownDlg::OnUpdateFrontdlg(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_bFront);
}

//实现开机启动。向注册表 HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run 写入自己实现自启
void CAutoShutdownDlg::OnStartup()
{
	// TODO: 在此添加命令处理程序代码
	CHAR FilePath[MAX_PATH];
	//这儿有问题。我是新手，不会数据间转换，只好用了GetModuleFileNameA，
	//用GetModuleFileName时使TCHAR数组类型向BYTE *转换发生问题。希望大神解答。pprain_yue@sina.com，非常感谢
	GetModuleFileNameA(NULL,FilePath,sizeof(FilePath));
	if (!m_bIsStartUp)
	{
		if(ERROR_SUCCESS != RegOpenKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",&hKey))
		{
			return;
		}
		else
		{
			//创建注册表项
			if (ERROR_SUCCESS != (::RegSetValueExA(hKey,"AutoShutDown2",0,REG_SZ,\
				(BYTE *)FilePath,sizeof(FilePath))))
			{
				RegCloseKey(hKey);
				return;
			} 
			else
			{
				RegCloseKey(hKey);
			}
		}
	} 
	else
	{
		if(ERROR_SUCCESS != RegOpenKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",&hKey))
		{
			return;
		}
		else
		{
			//删除注册表项
			::RegDeleteValue(hKey,L"AutoShutDown2");
			RegCloseKey(hKey);
		}
	}
	m_bIsStartUp = !m_bIsStartUp;
}


void CAutoShutdownDlg::OnUpdateStartup(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_bIsStartUp);
}


BOOL CAutoShutdownDlg::OnQueryEndSession()
{
	if (!CDialogEx::OnQueryEndSession())
		return FALSE;

	// TODO:  在此添加专用的查询结束会话代码
	KillTimer(ShutDownTimer);
	KillTimer(CountDownTimer);
	return TRUE;
}


void CAboutDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static int m_Count = 0;
	switch(m_Count%6)
	{
	case 0:
		m_HopeText.SetWindowText(L"漠漠轻寒上小楼\n晓阴无赖似穷秋\n淡烟流水画屏幽");
		break;
	case 1:
		m_HopeText.SetWindowText(L"晓阴无赖似穷秋\n淡烟流水画屏幽\n");
		break;
	case 2:
		m_HopeText.SetWindowText(L"淡烟流水画屏幽\n");
		break;
	case 3:
		m_HopeText.SetWindowText(L"自在飞花轻似梦\n无边丝雨细如愁\n宝帘闲挂小银钩");
		break;
	case 4:
		m_HopeText.SetWindowText(L"无边丝雨细如愁\n宝帘闲挂小银钩");
		break;
	case 5:
		m_HopeText.SetWindowText(L"宝帘闲挂小银钩\n");
		break;
	}
	m_Count++;
	CDialogEx::OnLButtonDown(nFlags, point);
}
