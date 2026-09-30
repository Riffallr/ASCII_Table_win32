#pragma once

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")



#define onExitAction 1
#define onInfoAction 2
#define onOpenSite 3



void drawAsciiTable(HDC hdc, RECT& rc, HWND hWnd);