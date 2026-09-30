#include "windows.h"
#include "resource.h"
#include "main.hpp"
#include "shellapi.h"
#include <stdio.h>


int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR szCmdLine, int nCmdShow) {
	MSG msg{};
	HWND hWnd{};
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) };

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hInstance = hInstance;
	wc.lpfnWndProc = [](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)->LRESULT {
		switch (uMsg)
		{
		case WM_CREATE: {

			HMENU hMenu = CreateMenu();
			HMENU hPopUp = CreatePopupMenu();

			AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hPopUp, L"File");
			AppendMenu(hPopUp, MF_STRING, onInfoAction, L"Info");
			AppendMenu(hPopUp, MF_STRING, onOpenSite, L"Socials");
			AppendMenu(hPopUp, MF_SEPARATOR, NULL, NULL);
			AppendMenu(hPopUp, MF_STRING, onExitAction, L"Exit");

			//HWND hText = CreateWindow(L"STATIC", L"ASCII TABLE", WS_CHILD | WS_VISIBLE | SS_SIMPLE, 350, 10, 100, 40, hWnd, nullptr, nullptr, nullptr);

			SetMenu(hWnd, hMenu);
			return 0;
		}
		break;
		case WM_PAINT: {
			PAINTSTRUCT ps;
			RECT rc;
			HDC hdc = BeginPaint(hWnd, &ps);
			//

			GetClientRect(hWnd, &rc);
			drawAsciiTable(hdc, rc, hWnd);



			//
			EndPaint(hWnd, &ps);
			return 0;
		}

		case WM_COMMAND:
			switch (wParam)
			{
			case onExitAction:
				PostQuitMessage(EXIT_SUCCESS);
				break;
			case onInfoAction:
				MessageBoxA(hWnd, "Author: Riffall\nYES", "Information", MB_ICONINFORMATION);
				break;
			case onOpenSite:
				ShellExecuteW(hWnd, L"open", L"https://guns.lol/riffall", nullptr, nullptr, SW_SHOWNORMAL);
				break;
			}
			return 0;
		case WM_DESTROY:
			PostQuitMessage(EXIT_SUCCESS);
			return 0;
			break;
		case WM_MOUSEWHEEL: {
			SCROLLINFO si = { sizeof(si) };
			si.fMask = SIF_ALL;
			GetScrollInfo(hWnd, SB_VERT, &si);

			int delta = GET_WHEEL_DELTA_WPARAM(wParam);
			int scrollStep = si.nPage / 10;
			int scrollAmount = (delta / WHEEL_DELTA) * scrollStep;

			int newPos = si.nPos - scrollAmount;

			if (newPos < si.nMin) newPos = si.nMin;
			if (newPos > si.nMax - (int)si.nPage) newPos = si.nMax - (int)si.nPage;

			si.fMask = SIF_POS;
			si.nPos = newPos;
			SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
			InvalidateRect(hWnd, nullptr, TRUE);
			return 0;
			break;
		}
		case WM_VSCROLL: {
			SCROLLINFO si = { sizeof(si) };
			si.fMask = SIF_ALL;
			GetScrollInfo(hWnd, SB_VERT, &si);

			int newPos = si.nPos;

			//
			switch (LOWORD(wParam))
			{
			case SB_LINEUP:
				newPos -= 20;
				break;
			case SB_LINEDOWN:
				newPos += 20;
				break;
			case SB_PAGEUP:
				newPos -= si.nPage;
				break;
			case SB_PAGEDOWN:
				newPos += si.nPage;
				break;
			case SB_THUMBTRACK:
				newPos = si.nTrackPos;
				break;
			}

			if (newPos < si.nMin) newPos = si.nMin;
			if (newPos > si.nMax - (int)si.nPage) newPos = si.nMax - (int)si.nPage;

			si.fMask = SIF_POS;
			si.nPos = newPos;
			SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
			
			InvalidateRect(hWnd, nullptr, TRUE);
			//

			return 0;
		}
		case WM_SIZE: {
			SCROLLINFO si = { sizeof(si) };
			si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
			si.nMin = 0;
			si.nMax = 800;
			si.nPage = HIWORD(lParam);
			si.nPos = 0;
			SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

			InvalidateRect(hWnd, nullptr, TRUE);
			return 0;
		}
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
			break;
		} //
	};
	wc.lpszClassName = L"MyClass";
	wc.lpszMenuName = nullptr;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc)) {
		return EXIT_FAILURE;
	}

	if (hWnd = CreateWindow(wc.lpszClassName, L"ASCII table", WS_OVERLAPPEDWINDOW | WS_VSCROLL, 40, 50, 800, 600, nullptr, nullptr, wc.hInstance, nullptr); hWnd == INVALID_HANDLE_VALUE) {
		return EXIT_FAILURE;
	}


	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return static_cast<int>(msg.wParam);
}


void drawAsciiTable(HDC hdc, RECT& rc, HWND hWnd) {
	SCROLLINFO si = { sizeof(si) };
	si.fMask = SIF_POS;
	GetScrollInfo(hWnd, SB_VERT, &si);
	int scrollY = si.nPos;

	HFONT hFontBold = CreateFont(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH | FF_MODERN, L"Verdana");
	HFONT hFont = CreateFont(0, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH | FF_MODERN, L"TimesNewRoman");
	SetBkMode(hdc, TRANSPARENT);

	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);

	int lineH = tm.tmHeight + tm.tmExternalLeading;
	int half = rc.right / 2;
	int padding = 4;

	int colW = half / 3;
	int xDec = padding;
	int xHex = colW + padding;
	int xChar = colW * 2 + padding;
	int yHead = padding - scrollY;

	int offsetLeft = 0;
	int offsetRight = half;


	HFONT hOldb = (HFONT)SelectObject(hdc, hFontBold);
	TextOutW(hdc, offsetLeft + xDec, yHead, L"Dec", 3);
	TextOutW(hdc, offsetLeft + xHex, yHead, L"Hex", 3);
	TextOutW(hdc, offsetLeft + xChar, yHead, L"Char", 4);

	TextOutW(hdc, offsetRight + xDec, yHead, L"Dec", 3);
	TextOutW(hdc, offsetRight + xHex, yHead, L"Hex", 3);
	TextOutW(hdc, offsetRight + xChar, yHead, L"Char", 4);

	int bodyY0 = yHead + lineH;
	int midIndex = 48;



	HFONT hOld = (HFONT)SelectObject(hdc, hFont);
	for (int i = 32; i < 127; i++) {


		int idx = i - 32;
		int offset; 
		int row;

		if (idx < midIndex) {
			offset = offsetLeft;
			row = idx;
		}
		else {
			offset = offsetRight;
			row = idx - midIndex;
		}

		int y = bodyY0 + row * lineH;

		wchar_t bufDec[8], bufHex[8], bufChar[4];
		swprintf_s(bufDec, L"%d", i);
		swprintf_s(bufHex, L"%X", i);
		swprintf_s(bufChar, L"%c", (wchar_t)i);

		TextOutW(hdc, offset + xDec, y, bufDec, wcslen(bufDec));
		TextOutW(hdc, offset + xHex, y, bufHex, wcslen(bufHex));
		TextOutW(hdc, offset + xChar, y, bufChar, wcslen(bufChar));
	}




	SelectObject(hdc, hOldb);
	DeleteObject(hFont);
	DeleteObject(hFontBold);
}


