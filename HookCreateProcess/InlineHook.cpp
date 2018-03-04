#include "stdafx.h"
#include "InlineHook.h"

CInlineHook::CInlineHook()
{
        // 对成员变量的初始化
        m_pfnOrig = NULL;
        ZeroMemory(m_bOldBytes, 5);
        ZeroMemory(m_bNewBytes, 5);
}

CInlineHook::~CInlineHook()
{
        // 取消Hook
        UnHook();
}

//挂钩函数，参数依次为模块名称、函数名称以及自定义的钩子函数
BOOL CInlineHook::Hook(LPSTR pszModuleName, LPSTR pszFuncName, PROC pfnHookFunc)
{
        BOOL bRet = FALSE;
    
        // 获取指定模块中函数的地址
        m_pfnOrig = (PROC)GetProcAddress(GetModuleHandle(pszModuleName), pszFuncName);

        if ( m_pfnOrig != NULL )
        {
                // 保存该地址处前5个字节的内容
                DWORD dwNum = 0;
                ReadProcessMemory(GetCurrentProcess(), m_pfnOrig, m_bOldBytes, 5, &dwNum);

                // 构造JMP指令，"\xe9"为jmp的Opcode
                m_bNewBytes[0] = '\xe9';    
                // pfnHookFunc是Hook后的地址，m_pfnOrig是原来的地址，5是指令长度
                *(DWORD *)(m_bNewBytes + 1) = (DWORD)pfnHookFunc - (DWORD)m_pfnOrig - 5;

                // 将构造好的地址写入该地址处
                WriteProcessMemory(GetCurrentProcess(), m_pfnOrig, m_bNewBytes, 5, &dwNum);

                bRet = TRUE;
        }
    
        return bRet;
}

//取消函数的挂钩
void CInlineHook::UnHook()
{
        if ( m_pfnOrig != 0 )
        {
                DWORD dwNum = 0;
                WriteProcessMemory(GetCurrentProcess(), m_pfnOrig, m_bOldBytes, 5, &dwNum);
        }
}

//重新对函数进行挂钩  
BOOL CInlineHook::ReHook()  
{  
        BOOL bRet = FALSE;  
  
        if ( m_pfnOrig != 0 )  
        {  
                DWORD dwNum = 0;  
                WriteProcessMemory(GetCurrentProcess(), m_pfnOrig, m_bNewBytes, 5, &dwNum);  
  
                bRet = TRUE;  
        }  
  
        return bRet;  
}  