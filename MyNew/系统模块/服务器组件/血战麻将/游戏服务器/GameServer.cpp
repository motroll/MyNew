#include "Stdafx.h"
#include <afxdllx.h>
#include "Randkiss.h"



//////////////////////////////////////////////////////////////////////////

static AFX_EXTENSION_MODULE GameServerDLL={NULL,NULL};

//添加随机数 phw
CRandKiss g_RandKiss;

//DLL 导出主函数
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	srand((unsigned)time(NULL));
	g_RandKiss.SrandKiss((unsigned)time(NULL));
	int num = 5 + rand() % 10;
	for (int i = 0; i < num; i++)
	{
		g_RandKiss.RandKiss();
	}

	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(GameServerDLL, hInstance)) return 0;
		new CDynLinkLibrary(GameServerDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(GameServerDLL);
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////
