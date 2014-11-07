#include "tray.h"

#include <windows.h>

static NOTIFYICONDATA nid;
static HWND dummyHWND;
static HBITMAP hBitmap = NULL, hBitmapMask = NULL;

static HICON create_icon(HWND hWnd, char *szText)
{
	HDC hdc, hdcMem;
	HBITMAP hOldBitMap = NULL;
	ICONINFO iconInfo;
	HFONT hFont;
	HICON hIcon;
	int bitmapWidth;

	bitmapWidth = GetSystemMetrics(SM_CXSMICON);
	hdc = GetDC(hWnd);
	hdcMem = CreateCompatibleDC(hdc);
	if(hBitmap == NULL) {
		hBitmap = CreateCompatibleBitmap(hdc, bitmapWidth, bitmapWidth);
	}
	if(hBitmapMask == NULL) {
		hBitmapMask = CreateCompatibleBitmap(hdc, bitmapWidth, bitmapWidth);
	}
	ReleaseDC(hWnd, hdc);
	hOldBitMap = (HBITMAP) SelectObject(hdcMem, hBitmap);

	SetBkColor(hdcMem, RGB(0x00, 0x00, 0x00));
	hFont = CreateFont(
		-MulDiv(11, GetDeviceCaps(hdcMem, LOGPIXELSY), 72),
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial")
	);
	hFont = (HFONT) SelectObject(hdcMem, hFont);
	SetTextColor(hdcMem, RGB(0x00, 0xFF, 0x00));
	TextOut(hdcMem, bitmapWidth / 4, 0, szText, lstrlen(szText));

	SelectObject(hdc, hOldBitMap);
	hOldBitMap = NULL;

	iconInfo.fIcon = TRUE;
	iconInfo.xHotspot = 0;
	iconInfo.yHotspot = 0;
	iconInfo.hbmMask = hBitmapMask;
	iconInfo.hbmColor = hBitmap;

	hIcon = CreateIconIndirect(&iconInfo);

	DeleteObject(SelectObject(hdcMem, hFont));
	DeleteDC(hdcMem);
	DeleteDC(hdc);

	return hIcon;
}

void trayicon_create()
{
	dummyHWND = CreateWindowA(
		"STATIC", "dummy",
		0, 0, 0, 0, 0,
		NULL, NULL, NULL, NULL
	);
	memset(&nid, 0, sizeof(nid));
	nid.cbSize = sizeof(nid);
	nid.hWnd = dummyHWND;
	nid.uID = 100;
	nid.uFlags = NIF_ICON;
	nid.hIcon = create_icon(dummyHWND, "1");
	Shell_NotifyIcon(NIM_ADD, &nid);
}

void trayicon_remove()
{
	DeleteObject(hBitmap);
	DeleteObject(hBitmapMask);
	Shell_NotifyIcon(NIM_DELETE, &nid);
}

void trayicon_set(int number)
{
	char snumber[2];
	if(number < 0 || number > 9) {
		return;
	}
	snumber[0] = number + '0';
	snumber[1] = '\0';
	nid.hIcon = create_icon(dummyHWND, snumber);
	Shell_NotifyIcon(NIM_MODIFY, &nid);
}
