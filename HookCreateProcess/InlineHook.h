#include <windows.h>

class CInlineHook
{
public:
        CInlineHook();      // 构造函数，用于初始化
        ~CInlineHook();     // 析构函数，用户程序结束后资源的释放

        // Hook函数
        BOOL Hook(LPSTR pszModuleName, LPSTR pszFuncName, PROC pfnHookFunc);
        // 取消Hook函数
        void UnHook();
		// 重新进行Hook函数
		BOOL ReHook();

private:
        PROC m_pfnOrig;         // 自定义的函数的地址
        BYTE m_bOldBytes[5];    // 原始函数入口代码
        BYTE m_bNewBytes[5];    // 构造的跳转指令的代码
};