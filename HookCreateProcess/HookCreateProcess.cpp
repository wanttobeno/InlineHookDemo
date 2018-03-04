// HookCreateProcess.cpp : Defines the entry point for the DLL application.
//
#include "windows.h"
#include "stdafx.h"
#include "InlineHook.h"

// 创建一个名为CreateProcessHook的CInlineHook类
CInlineHook CreateProcessHook;

// 我们实现的Hook函数
BOOL WINAPI MyCreateProcessW(
    LPCWSTR               lpApplicationName,
    LPWSTR                lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL                  bInheritHandles,
    DWORD                 dwCreationFlags,
    LPVOID                lpEnvironment,
    LPCWSTR               lpCurrentDirectory,
    LPSTARTUPINFOW        lpStartupInfo,
    LPPROCESS_INFORMATION lpProcessInformation
    )
{
    BOOL bRet = FALSE;
	// 如果启动的程序不是wscript.exe，则直接放行
	BOOL bFind = FALSE;
	if (lpApplicationName)
	{
		if( wcsicmp(lpApplicationName, L"c:\\windows\\system32\\wscript.exe") == 0 )
			bFind = TRUE;
	}
	if (lpCommandLine)
	{
		if(wcsstr(wcslwr(lpCommandLine), L"c:\\windows\\system32\\wscript.exe"))
			bFind = TRUE;
	}
	if (bFind == FALSE)
    {	
		CreateProcessHook.UnHook();
        bRet = CreateProcessW(lpApplicationName,
                        lpCommandLine,
                        lpProcessAttributes,
                        lpThreadAttributes,
                        bInheritHandles,
                        dwCreationFlags,
                        lpEnvironment,
                        lpCurrentDirectory,
                        lpStartupInfo,
                        lpProcessInformation);
         CreateProcessHook.ReHook();
    } 
	else
    {
		if(MessageBox(NULL, "检测到脚本程序已经启动，是否拦截？", "重要提示", MB_YESNO) == IDNO)
		{
			CreateProcessHook.UnHook();
			bRet = CreateProcessW(lpApplicationName,
                        lpCommandLine,
                        lpProcessAttributes,
                        lpThreadAttributes,
                        bInheritHandles,
                        NULL,  // 以挂起的方式建立wscript.exe进程
                        lpEnvironment,
                        lpCurrentDirectory,
                        lpStartupInfo,
                        lpProcessInformation);
			CreateProcessHook.ReHook();
		}
		else
		{
			MessageBox(NULL, "脚本程序已拦截", "重要提示",MB_OK);
		}
    }
    
    return bRet;
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                      DWORD  ul_reason_for_call, 
                      LPVOID lpReserved
                      )
{
    switch ( ul_reason_for_call )
    {
    case DLL_PROCESS_ATTACH:
        {
            // Hook CreateProcessW()函数
            CreateProcessHook.Hook("kernel32.dll",
                "CreateProcessW",
                (PROC)MyCreateProcessW);
            break;
        }
    case DLL_PROCESS_DETACH:
        {
            CreateProcessHook.UnHook();
            break;
        }
    }
    
    return TRUE;
}