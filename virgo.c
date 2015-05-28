#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define sb_free(a)   ((a) ? free(stb__sbraw(a)),0 : 0)
#define sb_push(a,v) (stb__sbmaybegrow(a,1), (a)[stb__sbn(a)++] = (v))
#define sb_count(a)  ((a) ? stb__sbn(a) : 0)

#define stb__sbraw(a) ((int *) (a) - 2)
#define stb__sbm(a)   stb__sbraw(a)[0]
#define stb__sbn(a)   stb__sbraw(a)[1]

#define stb__sbneedgrow(a,n)  ((a)==0 || stb__sbn(a)+(n) >= stb__sbm(a))
#define stb__sbmaybegrow(a,n) (stb__sbneedgrow(a,(n)) ? stb__sbgrow(a,n) : 0)
#define stb__sbgrow(a,n)      ((a) = stb__sbgrowf((a), (n), sizeof(*(a))))

#ifndef MOD_NOREPEAT
#define MOD_NOREPEAT 0x4000
#endif

#define NUM_DESKTOPS 4

typedef struct {
	HWND *windows;
	int count;
} Windows;

typedef struct {
	NOTIFYICONDATA nid;
	HWND dummyHWND;
	HDC hdc;
	HBITMAP hBitmap;
	int bitmapWidth;
} Trayicon;

typedef struct {
	int current;
	int handle_hotkeys;
	Windows desktops[NUM_DESKTOPS];
	Trayicon trayicon;
} Virgo;

static void *stb__sbgrowf(void *arr, int increment, int itemsize)
{
	int dbl_cur = arr ? 2*stb__sbm(arr) : 0;
	int min_needed = sb_count(arr) + increment;
	int m = dbl_cur > min_needed ? dbl_cur : min_needed;
	int *p = realloc(arr ? stb__sbraw(arr) : 0, itemsize * m + sizeof(int)*2);
	if(p) {
		if(!arr) {
			p[1] = 0;
		}
		p[0] = m;
		return p+2;
	} else {
		ExitProcess(1);
		return (void *)(2*sizeof(int));
	}
}

static HICON trayicon_draw(Trayicon *t, char *text, int len)
{
	HICON hIcon;
	HFONT hFont;
	HBITMAP hOldBitMap;
	HDC hdcMem;
	ICONINFO iconInfo = {TRUE, 0, 0, t->hBitmap, t->hBitmap};
	hdcMem = CreateCompatibleDC(t->hdc);
	SetBkColor(hdcMem, RGB(0x00, 0x00, 0x00));
	hOldBitMap = (HBITMAP) SelectObject(hdcMem, t->hBitmap);
	hFont = CreateFont(
		-MulDiv(11, GetDeviceCaps(hdcMem, LOGPIXELSY), 72),
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial")
	);
	hFont = (HFONT) SelectObject(hdcMem, hFont);
	SetTextColor(hdcMem, RGB(0x00, 0xFF, 0x00));
	TextOut(hdcMem, t->bitmapWidth / 4, 0, text, len);
	SelectObject(hdcMem, hOldBitMap);
	hOldBitMap = NULL;
	hIcon = CreateIconIndirect(&iconInfo);
	DeleteObject(SelectObject(hdcMem, hFont));
	DeleteDC(hdcMem);
	return hIcon;
}

static void trayicon_init(Trayicon *t)
{
	t->dummyHWND = CreateWindowA(
		"STATIC", "dummy",
		0, 0, 0, 0, 0,
		NULL, NULL, NULL, NULL
	);
	t->hdc = GetDC(t->dummyHWND);
	t->bitmapWidth = GetSystemMetrics(SM_CXSMICON);
	t->hBitmap = CreateCompatibleBitmap(t->hdc, t->bitmapWidth, t->bitmapWidth);
	memset(&t->nid, 0, sizeof(t->nid));
	t->nid.cbSize = sizeof(t->nid);
	t->nid.hWnd = t->dummyHWND;
	t->nid.uID = 100;
	t->nid.uFlags = NIF_ICON;
	t->nid.hIcon = trayicon_draw(t, "1", 1);
	Shell_NotifyIcon(NIM_ADD, &t->nid);
}

static void trayicon_set(Trayicon *t, int number)
{
	char snumber[2];
	if(!(number>=0 && number<=9)) {
		return;
	}
	snumber[0] = number + '0';
	snumber[1] = '\0';
	t->nid.hIcon = trayicon_draw(t, snumber, 1);
	Shell_NotifyIcon(NIM_MODIFY, &t->nid);
}

static void trayicon_deinit(Trayicon *t)
{
	DeleteObject(t->hBitmap);
	ReleaseDC(t->dummyHWND, t->hdc);
	Shell_NotifyIcon(NIM_DELETE, &t->nid);
	DestroyWindow(t->dummyHWND);
}

static void windows_mod(Windows *wins, int state)
{
	int i;
	for(i=0; i<wins->count; i++) {
		ShowWindow(wins->windows[i], state);
	}
}

static void windows_show(Windows *wins)
{
	windows_mod(wins, SW_SHOW);
}

static void windows_hide(Windows *wins)
{
	windows_mod(wins, SW_HIDE);
}

static void windows_add(Windows *wins, HWND hwnd)
{
	if(wins->count >= sb_count(wins->windows)) {
		sb_push(wins->windows, hwnd);
	} else {
		wins->windows[wins->count] = hwnd;
	}
	wins->count++;
}

static void windows_del(Windows *wins, HWND hwnd)
{
	int i, m;
	for(i=0; i<wins->count; i++) {
		if(wins->windows[i] != hwnd) {
			continue;
		}
		m = wins->count-i-1;
		if(m > 0) {
			memcpy(
				&(wins->windows[i]),
				&(wins->windows[i+1]),
				sizeof(HWND)*m
			);
		}
		wins->count--;
		break;		
	}
}

static int is_valid_window(HWND hwnd)
{
	return IsWindowVisible(hwnd)
		&& hwnd!=FindWindow("Progman", NULL)
		&& hwnd!=FindWindow("Shell_TrayWnd", NULL);
}

static void register_hotkey(int id, int mod, int vk)
{
	if(!RegisterHotKey(NULL, id, mod, vk)) {
		MessageBox(NULL, "could not register hotkey", "error",
			MB_ICONEXCLAMATION);
		ExitProcess(1);
	}
}

static BOOL enum_func(HWND hwnd, LPARAM lParam)
{
	int i, e;
	Virgo *v;
	Windows *desk;
	v = (Virgo *) lParam;
	if(!is_valid_window(hwnd)) {
		return 1;
	}
	for(i=0; i<NUM_DESKTOPS; i++) {
		desk = &(v->desktops[i]);
		for(e=0; e<desk->count; e++) {
			if(desk->windows[e] == hwnd) {
				return 1;
			}
		}
	}
	windows_add(&(v->desktops[v->current]), hwnd);
	return 1;
}

static void virgo_update(Virgo *v)
{
	int i, e;
	Windows *desk;
	HWND hwnd;
	for(i=0; i<NUM_DESKTOPS; i++) {
		desk = &(v->desktops[i]);
		for(e=0; e<desk->count; e++) {
			hwnd = desk->windows[e];
			if(!GetWindowThreadProcessId(desk->windows[e], NULL)) {
				windows_del(desk, hwnd);
			}
		}
	}
	desk = &v->desktops[v->current];
	for(i=0; i<desk->count; i++) {
		hwnd = desk->windows[i];
		if(!IsWindowVisible(hwnd)) {
			windows_del(desk, hwnd);
		}
	}
	EnumWindows((WNDENUMPROC)&enum_func, (LPARAM)v);
}

static void virgo_toggle_hotkeys(Virgo *v)
{
	int i;
	v->handle_hotkeys = !v->handle_hotkeys;
	if(v->handle_hotkeys) {
		for(i=0; i<NUM_DESKTOPS; i++) {
			register_hotkey(i*2, MOD_ALT|MOD_NOREPEAT, i+1+0x30);
			register_hotkey(i*2+1, MOD_CONTROL|MOD_NOREPEAT, i+1+0x30);
		}
	} else {
		for(i=0; i<NUM_DESKTOPS; i++) {
			UnregisterHotKey(NULL, i*2);
			UnregisterHotKey(NULL, i*2+1);
		}
	}
}

static void virgo_init(Virgo *v)
{
	int i;
	v->current = 0;
	v->handle_hotkeys = 1;
	for(i=0; i<NUM_DESKTOPS; i++) {
		v->desktops[i].windows = NULL;
		v->desktops[i].count = 0;
		register_hotkey(i*2, MOD_ALT|MOD_NOREPEAT, i+1+0x30);
		register_hotkey(i*2+1, MOD_CONTROL|MOD_NOREPEAT, i+1+0x30);
	}
	register_hotkey(i*2, MOD_ALT|MOD_CONTROL|MOD_SHIFT|MOD_NOREPEAT, 'Q');
	register_hotkey(i*2+1, MOD_ALT|MOD_CONTROL|MOD_SHIFT|MOD_NOREPEAT, 'S');
	trayicon_init(&v->trayicon);
}

static void virgo_deinit(Virgo *v)
{
	int i;
	for(i=0; i<NUM_DESKTOPS; i++) {
		windows_show(&v->desktops[i]);
		sb_free(v->desktops[i].windows);
	}
	trayicon_deinit(&v->trayicon);
}

static void virgo_move_to_desk(Virgo *v, int desk)
{
	HWND hwnd;
	if(v->current == desk) {
		return;
	}
	virgo_update(v);
	hwnd = GetForegroundWindow();
	if(!hwnd || !is_valid_window(hwnd)) {
		return;
	}
	windows_del(&v->desktops[v->current], hwnd);
	windows_add(&v->desktops[desk], hwnd);
	ShowWindow(hwnd, SW_HIDE);
}

static void virgo_go_to_desk(Virgo *v, int desk)
{
	if(v->current == desk) {
		return;
	}
	virgo_update(v);
	windows_hide(&v->desktops[v->current]);
	windows_show(&v->desktops[desk]);
	v->current = desk;
	trayicon_set(&v->trayicon, v->current+1);
}

extern void __main(void) asm("__main");
void __main(void)
{
	Virgo v;
	MSG msg;
	virgo_init(&v);
	while(GetMessage(&msg, NULL, 0, 0)) {
		if(msg.message != WM_HOTKEY) {
			continue;
		}
		if(msg.wParam == NUM_DESKTOPS*2) {
			break;
		}
		if(msg.wParam == NUM_DESKTOPS*2+1) {
			virgo_toggle_hotkeys(&v);
		} else if(msg.wParam%2 == 0) {
			virgo_go_to_desk(&v, msg.wParam/2);
		} else {
			virgo_move_to_desk(&v, (msg.wParam-1)/2);
		}
	}
	virgo_deinit(&v);
	ExitProcess(0);
}