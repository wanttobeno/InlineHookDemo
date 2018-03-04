// InjectDllDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InjectDll.h"
#include "InjectDllDlg.h"
#include <tlhelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInjectDllDlg dialog

CInjectDllDlg::CInjectDllDlg(CWnd* pParent /*=NULL*/)
: CDialog(CInjectDllDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInjectDllDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInjectDllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInjectDllDlg)
	DDX_Control(pDX, IDC_EDIT_DLLFILE2, m_editDllFile2);
	DDX_Control(pDX, IDC_EDIT_DLLFILE, m_editDllFile);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInjectDllDlg, CDialog)
//{{AFX_MSG_MAP(CInjectDllDlg)
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_BTN_INJECT, OnBtnInject)
ON_BN_CLICKED(IDC_BTN_UNINJECT, OnBtnUninject)
ON_BN_CLICKED(IDC_BTN_INJECT2, OnBtnInject2)
ON_BN_CLICKED(IDC_BTN_UNINJECT2, OnBtnUninject2)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInjectDllDlg message handlers

BOOL CInjectDllDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	typedef BOOL ( WINAPI * MsMessageFilter)(UINT message,DWORD dwFlag);
	HMODULE hMod = LoadLibrary("user32.dll");
	if (hMod)
	{
		MsMessageFilter  MsChangeWinMessageFilter;
		MsChangeWinMessageFilter = (MsMessageFilter)GetProcAddress(hMod,"ChangeWindowMessageFilter");
		if (MsChangeWinMessageFilter)
		{
			MsChangeWinMessageFilter(WM_DROPFILES, 1);
			MsChangeWinMessageFilter(0x0049, 1);// 0x0049 == WM_COPYGLOBALDATA
		}
		FreeLibrary(hMod);
	}

    DebugPrivilege();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CInjectDllDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CInjectDllDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CInjectDllDlg::DebugPrivilege()
{
    HANDLE hToken = NULL;
	
    BOOL bRet = OpenProcessToken(GetCurrentProcess(), 
		TOKEN_ALL_ACCESS, &hToken);
	
    if ( bRet == TRUE )
    {
        TOKEN_PRIVILEGES tp;
		
        tp.PrivilegeCount = 1;
        LookupPrivilegeValue(NULL, SE_DEBUG_NAME, 
			&tp.Privileges[0].Luid);
		
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        AdjustTokenPrivileges(hToken, FALSE, &tp, 
			sizeof(tp), NULL, NULL);
		
        CloseHandle(hToken);
    }
}

void CInjectDllDlg::OnBtnInject() 
{
	// TODO: Add your control notification handler code here
    char szDllName[MAX_PATH] = { 0 };
    char szProcessName[MAXBYTE] = { 0 };
    DWORD dwPid = 0;
	
    GetDlgItemText(IDC_EDIT_DLLFILE, szDllName, MAX_PATH);
    GetDlgItemText(IDC_EDIT_PROCESSNAME, szProcessName, MAXBYTE);
	if (strlen(szDllName) == 0 || strlen(szProcessName)==0)
		return;
    // 由进程名获得PID
    dwPid = GetProcId(szProcessName);
	if (dwPid != 0)
	{
		// 注入szDllName到dwPid
		if(InjectDll(dwPid, szDllName))
			::MessageBox(m_hWnd,_T("注入成功"),_T("提示"),MB_OK);
		else
			::MessageBox(m_hWnd,_T("注入失败"),_T("提示"),MB_OK);
	}
	else
	{
		::MessageBox(m_hWnd,_T("进程不存在"),_T("提示"),MB_OK);
	}
}

DWORD CInjectDllDlg::GetProcId(char *szProcessName)
{
    BOOL bRet;
    PROCESSENTRY32 pe32;
    HANDLE hSnap;
	
    hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    pe32.dwSize = sizeof(pe32);
    bRet = Process32First(hSnap, &pe32);
	
    while ( bRet )
    {
        // strupr()函数是将字符串转化为大写
        if ( lstrcmp(strupr(pe32.szExeFile), 
			strupr(szProcessName)) == 0 )
        {
            return pe32.th32ProcessID;
        }
		
        bRet = Process32Next(hSnap, &pe32);
    }
	//CloseToolhelp32Snapshot(hSnap);
    return 0;
}

BOOL CInjectDllDlg::InjectDll(DWORD dwPid, char *szDllName)
{
    if ( dwPid == 0 || lstrlen(szDllName) == 0 )
    {
        return FALSE;
    }
	
    char *pFunName = "LoadLibraryA";
	
    // 打开目标进程
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 
		FALSE, dwPid);
	
    if ( hProcess == NULL )
    {
        return FALSE;
    }
	
    // 计算欲注入DLL文件完整路径的长度
    int nDllLen = lstrlen(szDllName) + sizeof(char);
	
    // 在目标进程申请一块长度为nDllLen大小的内存空间
    PVOID pDllAddr = VirtualAllocEx(hProcess,
		NULL, nDllLen,
		MEM_COMMIT,
		PAGE_READWRITE);
	
    if ( pDllAddr == NULL )
    {
        CloseHandle(hProcess);
        return FALSE;
    }
	
    DWORD dwWriteNum = 0;
	
    // 将欲注入DLL文件的完整路径写入在目标进程中申请的空间内
    WriteProcessMemory(hProcess, pDllAddr, szDllName, 
		nDllLen, &dwWriteNum);
	
    // 获得LoadLibraryA()函数的地址
    FARPROC pFunAddr = GetProcAddress(GetModuleHandle("kernel32.dll"),
		pFunName);
	
    // 创建远程线程
    HANDLE hThread = CreateRemoteThread(hProcess,
		NULL, 0,
		(LPTHREAD_START_ROUTINE)pFunAddr,
		pDllAddr, 0, NULL);
	BOOL bResult = hThread==NULL?FALSE:TRUE;
    WaitForSingleObject(hThread, INFINITE);
	
    CloseHandle(hThread);
    CloseHandle(hProcess);
	return bResult;
}

void CInjectDllDlg::OnBtnUninject() 
{
    // TODO: Add your control notification handler code here
    char szDllName[MAX_PATH] = { 0 };
    char szProcessName[MAXBYTE] = { 0 };
    DWORD dwPid = 0;
    
    GetDlgItemText(IDC_EDIT_DLLFILE, szDllName, MAX_PATH);
    GetDlgItemText(IDC_EDIT_PROCESSNAME, szProcessName, MAXBYTE);
    if (strlen(szDllName) == 0 || strlen(szProcessName)==0)
		return;
    // 由进程名获得PID
    dwPid = GetProcId(szProcessName);
	if (dwPid !=0 )
	{
		// 注入szDllName到dwPid
		UnInjectDll(dwPid, szDllName);
	}
	else
	{
		::MessageBox(m_hWnd,_T("进程不存在"),_T("提示"),MB_OK);
	}
}

BOOL CInjectDllDlg::UnInjectDll(DWORD dwPid, char *szDllName)
{
    if ( dwPid == 0 || lstrlen(szDllName) == 0 )
    {
        return FALSE;
    }
	
    HANDLE hSnap = CreateToolhelp32Snapshot(
		TH32CS_SNAPMODULE, 
		dwPid);
	
    MODULEENTRY32 me32;
    me32.dwSize = sizeof(me32);
	
    // 查找匹配的进程名称
    BOOL bRet = Module32First(hSnap, &me32);
    while ( bRet )
    {
        if ( lstrcmp(strupr(me32.szExePath), 
			strupr(szDllName)) == 0 )
        {
            break;
        }
		
        bRet = Module32Next(hSnap, &me32);
    }
	
    CloseHandle(hSnap);
	
    char *pFunName = "FreeLibrary";
	
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 
		FALSE, dwPid);
    if ( hProcess == NULL )
    {
        return FALSE;
    }
	
    FARPROC pFunAddr = GetProcAddress(GetModuleHandle("kernel32.dll"),
		pFunName);
	
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, 
		(LPTHREAD_START_ROUTINE)pFunAddr,
		me32.hModule, 0, NULL);
	BOOL bResult = hThread==NULL?FALSE:TRUE;
    WaitForSingleObject(hThread, INFINITE);
	
    CloseHandle(hThread);
    CloseHandle(hProcess);
	return bResult;
}

void CInjectDllDlg::OnBtnInject2() 
{
	// TODO: Add your control notification handler code here
    char szDllName[MAX_PATH] = { 0 };
    char szProcessName[MAXBYTE] = { 0 };
    DWORD dwPid = 0;
	
    GetDlgItemText(IDC_EDIT_DLLFILE2, szDllName, MAX_PATH);
    GetDlgItemText(IDC_EDIT_PROCESSNAME2, szProcessName, MAXBYTE);
	if (strlen(szDllName) == 0 || strlen(szProcessName)==0)
		return;
    // 由进程名获得PID
    dwPid = GetProcId(szProcessName);
	if (dwPid !=0)
	{
		// 注入szDllName到dwPid
		if(InjectDll(dwPid, szDllName))
			::MessageBox(m_hWnd,_T("注入成功"),_T("提示"),MB_OK);
		else
			::MessageBox(m_hWnd,_T("注入失败"),_T("提示"),MB_OK);
	}
	else
	{
		::MessageBox(m_hWnd,_T("进程不存在"),_T("提示"),MB_OK);
	}
}

void CInjectDllDlg::OnBtnUninject2() 
{
    // TODO: Add your control notification handler code here
    char szDllName[MAX_PATH] = { 0 };
    char szProcessName[MAXBYTE] = { 0 };
    DWORD dwPid = 0;
    
    GetDlgItemText(IDC_EDIT_DLLFILE2, szDllName, MAX_PATH);
    GetDlgItemText(IDC_EDIT_PROCESSNAME2, szProcessName, MAXBYTE);
    
	if (strlen(szDllName) == 0 || strlen(szProcessName)==0)
		return;
    // 由进程名获得PID
    dwPid = GetProcId(szProcessName);
	if (dwPid != 0)
	{
		// 注入szDllName到dwPid
		UnInjectDll(dwPid, szDllName);
	}
	else
	{
		::MessageBox(m_hWnd,_T("进程不存在"),_T("提示"),MB_OK);
	}
}
