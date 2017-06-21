
// AutoShutdownDlg.h : 头文件
//
#include <Mmsystem.h.>

#pragma once


// CAutoShutdownDlg 对话框
class CAutoShutdownDlg : public CDialogEx
{
// 构造
public:
	CAutoShutdownDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_AUTOSHUTDOWN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	BOOL WinShutDown(UINT uFags);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	afx_msg void OnAbout();
	afx_msg void OnQuit();
	afx_msg void OnAuthor();
	afx_msg void OnStartup();
	afx_msg void OnUpdateStartup(CCmdUI *pCmdUI);
	afx_msg void OnSelecttiming();
	afx_msg void OnSelectminute();
	afx_msg void OnBnClickedStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStop();
	afx_msg void OnMinidlg();
	afx_msg void OnShutdown();
	afx_msg void OnReboot();
	afx_msg void OnLogout();
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnClock();
	afx_msg void OnUpdateClock(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShutdown(CCmdUI *pCmdUI);
	afx_msg void OnUpdateReboot(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLogout(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStart(CCmdUI *pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnTraydlg();
	afx_msg void OnClose();
	afx_msg void OnMainapp();
	afx_msg void OnFrontdlg();
	afx_msg void OnUpdateFrontdlg(CCmdUI *pCmdUI);
	afx_msg BOOL OnQueryEndSession();

	//控件变量
	CEdit m_CountDown;
	CButton m_SelectTiming;
	CDateTimeCtrl m_TimingTime;
	CEdit m_TimingMinute;
	CEdit m_CurrentTime;
	CDateTimeCtrl m_TimingDate;
	CEdit m_CurrentDate;
	CMenu menu;
	CMenu *ntfMenu;
	CButton m_Custom;

	CRect rectLarge,rectSmall,rectSeparate;  //窗口最大的信息，窗口最小的信息，
	NOTIFYICONDATA ntfId;  //托盘图标结构体
	CString strTips;  //托盘图标显示的提示
	LONGLONG m_LongTime;  //倒计时秒数
	short m_Mode;  //到达指定时间，进行的响应模式
	HKEY hKey;
	MCI_OPEN_PARMS open;
	DWORD DeviceID;  //声音设备ID
	CString strSoundPath;  //自定义音乐路径

	bool m_bSelect;  //标志定时方式，确定时间定时和延迟定时
	bool m_Status;  //是否启动定时
	bool m_bFront;  //是否最前端显示
	bool m_bIsDlgNormal;  //窗口是否正常显示
	bool m_bIsSound;  //是否正在播放音乐
	bool m_bCustomSound;  //是否选择自定义音乐
	bool m_bIsStartUp;  //是否自动关机
};
