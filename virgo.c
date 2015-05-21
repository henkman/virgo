typedef void* HANDLE;
typedef HANDLE HWND;
typedef HANDLE HDC;
typedef HANDLE HBITMAP;
typedef HANDLE HICON;
typedef char TCHAR;
typedef HANDLE HFONT;
typedef int BOOL;
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef unsigned long DWORD;
typedef unsigned int UINT;
typedef long LONG;
typedef long LONG_PTR;
typedef unsigned int UINT_PTR;
typedef HANDLE HGDIOBJ;
typedef HANDLE HINSTANCE;
typedef HANDLE HMENU;
typedef WORD ATOM;
typedef unsigned long ULONG_PTR;
typedef ULONG_PTR SIZE_T;

typedef struct GUID {
  DWORD Data1;
  WORD  Data2;
  WORD  Data3;
  BYTE  Data4[8];
} GUID;

typedef struct NOTIFYICONDATA {
  DWORD cbSize;
  HWND  hWnd;
  UINT  uID;
  UINT  uFlags;
  UINT  uCallbackMessage;
  HICON hIcon;
  TCHAR szTip[64];
  DWORD dwState;
  DWORD dwStateMask;
  TCHAR szInfo[256];
  union {
    UINT uTimeout;
    UINT uVersion;
  };
  TCHAR szInfoTitle[64];
  DWORD dwInfoFlags;
  GUID  guidItem;
  HICON hBalloonIcon;
} NOTIFYICONDATA;

typedef struct ICONINFO {
  BOOL    fIcon;
  DWORD   xHotspot;
  DWORD   yHotspot;
  HBITMAP hbmMask;
  HBITMAP hbmColor;
} ICONINFO;

typedef struct POINT {
  LONG x;
  LONG y;
} POINT;

typedef struct MSG {
  HWND   hwnd;
  UINT   message;
  UINT_PTR wParam;
  LONG_PTR lParam;
  DWORD  time;
  POINT  pt;
} MSG;

typedef struct RECT {
  LONG left;
  LONG top;
  LONG right;
  LONG bottom;
} RECT;

typedef struct WINDOWINFO {
  DWORD cbSize;
  RECT  rcWindow;
  RECT  rcClient;
  DWORD dwStyle;
  DWORD dwExStyle;
  DWORD dwWindowStatus;
  UINT  cxWindowBorders;
  UINT  cyWindowBorders;
  ATOM  atomWindowType;
  WORD  wCreatorVersion;
} WINDOWINFO;

extern __stdcall HDC CreateCompatibleDC(HDC hdc);
typedef DWORD COLORREF;
extern __stdcall COLORREF SetBkColor(HDC hdc, COLORREF crColor);
extern __stdcall HFONT CreateFontA(
  int     nHeight,
  int     nWidth,
  int     nEscapement,
  int     nOrientation,
  int     fnWeight,
  DWORD   fdwItalic,
  DWORD   fdwUnderline,
  DWORD   fdwStrikeOut,
  DWORD   fdwCharSet,
  DWORD   fdwOutputPrecision,
  DWORD   fdwClipPrecision,
  DWORD   fdwQuality,
  DWORD   fdwPitchAndFamily,
  char * lpszFace
);
extern __stdcall HGDIOBJ SelectObject(
  HDC     hdc,
  HGDIOBJ hgdiobj
);
extern __stdcall int GetDeviceCaps(
  HDC hdc,
  int nIndex
);
extern __stdcall int MulDiv(
  int nNumber,
  int nNumerator,
  int nDenominator
);
extern __stdcall COLORREF SetTextColor(
   HDC      hdc,
   COLORREF crColor
);
extern __stdcall BOOL TextOutA(
   HDC     hdc,
   int     nXStart,
   int     nYStart,
   char * lpString,
   int     cchString
);

#define RGB(r,g,b) ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

extern __stdcall HICON CreateIconIndirect(
  ICONINFO *piconinfo
);
extern __stdcall BOOL DeleteObject(
  HGDIOBJ hObject
);
extern __stdcall BOOL DeleteDC(
  HDC hdc
);
extern __stdcall HWND CreateWindowExA(
	DWORD     dwExStyle,
  char *   lpClassName,
  char *  lpWindowName,
   DWORD     dwStyle,
   int       x,
   int       y,
   int       nWidth,
   int       nHeight,
   HWND      hWndParent,
   HMENU     hMenu,
   HINSTANCE hInstance,
   void *    lpParam
);
extern __stdcall HDC GetDC(
    HWND hWnd
);
extern __stdcall int  GetSystemMetrics(
   int nIndex
);
extern __stdcall HBITMAP CreateCompatibleBitmap(
    HDC hdc,
    int nWidth,
    int nHeight
);
extern __stdcall BOOL Shell_NotifyIcon(
   DWORD           dwMessage,
   NOTIFYICONDATA *lpdata
);
extern __stdcall int ReleaseDC(
    HWND hWnd,
    HDC  hDC
);
static int SW_HIDE = 0;
static int SW_SHOW = 5;
extern __stdcall BOOL  ShowWindow(
   HWND hWnd,
   int  nCmdShow
);

extern __stdcall HANDLE GetProcessHeap(void);

extern __stdcall void *HeapAlloc(
   HANDLE hHeap,
   DWORD  dwFlags,
   SIZE_T dwBytes
);
extern __stdcall void *HeapReAlloc(
   HANDLE hHeap,
   DWORD  dwFlags,
  void * lpMem,
   SIZE_T dwBytes
);
extern __stdcall void CopyMemory(
         void *  Destination,
   void *Source,
         SIZE_T Length
);
extern __stdcall BOOL HeapFree(
   HANDLE hHeap,
   DWORD  dwFlags,
   void *lpMem
);
extern __stdcall BOOL GetWindowInfo(
     HWND        hwnd,
   WINDOWINFO *pwi
);
extern __stdcall BOOL RegisterHotKey(
   HWND hWnd,
       int  id,
       UINT fsModifiers,
       UINT vk
);

extern __stdcall int MessageBoxA(
   HWND    hWnd,
   char * lpText,
   char * lpCaption,
       UINT    uType
);
extern __stdcall void ExitProcess(
   UINT uExitCode
);
extern __stdcall DWORD GetWindowThreadProcessId(
        HWND    hWnd,
   WORD *lpdwProcessId
);

typedef __stdcall BOOL (*WNDENUMPROC) (HWND hwnd, LONG_PTR lParam);
extern __stdcall BOOL EnumWindows(
   WNDENUMPROC lpEnumFunc,
   LONG_PTR      lParam
);

extern __stdcall HWND GetForegroundWindow(void);
extern __stdcall BOOL GetMessageA(
  MSG *lpMsg,
  HWND  hWnd,
       UINT  wMsgFilterMin,
       UINT  wMsgFilterMax
);

#define SM_CXSMICON 49

#define MB_ICONEXCLAMATION 0x00000030L

#define MOD_ALT 0x0001
#define MOD_CONTROL 0x0002
#define MOD_NOREPEAT 0x4000
#define MOD_SHIFT 0x0004
#define LOGPIXELSY 90

#define WM_HOTKEY                       0x0312
#define WS_VISIBLE 0x10000000L
#define WS_EX_TOOLWINDOW 0x00000080L
#define NIF_ICON (0x00000002)
#define NIM_ADD (0x00000000)
#define NIM_MODIFY (0x00000001)
#define NIM_DELETE (0x00000002)

typedef unsigned int size_t;
#define NULL ((void*)0)

/////////////////////////////////////////////

#define NUM_DESKTOPS 4

static void memset(void *dst, unsigned char v, size_t n)
{
	unsigned char *p = dst;
	while(n--) *p++ = v;
}

static void memcpy(void *dst, void *src, size_t n)
{
	unsigned char *d = dst;
	unsigned char *s = src;
	while(n--) *d++ = *s++;
}

typedef struct {
	HWND *windows;
	int count;
	int capacity;
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
	Windows desktops[NUM_DESKTOPS];
	Trayicon trayicon;
} Virgo;

static HICON trayicon_draw(Trayicon *t, char *text, int len)
{
	HICON hIcon;
	HFONT hFont;
	HBITMAP hOldBitMap;
	HDC hdcMem;
	ICONINFO iconInfo = {1, 0, 0, t->hBitmap, t->hBitmap};
	hdcMem = CreateCompatibleDC(t->hdc);
	SetBkColor(hdcMem, RGB(0x00, 0x00, 0x00));
	hOldBitMap = (HBITMAP) SelectObject(hdcMem, t->hBitmap);
	hFont = CreateFontA(
		-MulDiv(11, GetDeviceCaps(hdcMem, LOGPIXELSY), 72),
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Arial"
	);
	hFont = (HFONT) SelectObject(hdcMem, hFont);
	SetTextColor(hdcMem, RGB(0x00, 0xFF, 0x00));
	TextOutA(hdcMem, t->bitmapWidth / 4, 0, text, len);
	SelectObject(hdcMem, hOldBitMap);
	hOldBitMap = NULL;
	hIcon = CreateIconIndirect(&iconInfo);
	DeleteObject(SelectObject(hdcMem, hFont));
	DeleteDC(hdcMem);
	return hIcon;
}

static void trayicon_init(Trayicon *t)
{
	t->dummyHWND = CreateWindowExA(0,
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
	if(wins->count == wins->capacity) {
		if(wins->windows == NULL) {
			wins->capacity = 16;
			wins->windows = HeapAlloc(GetProcessHeap(),
				0, (SIZE_T)(sizeof(HWND)*wins->capacity));
		} else {
			wins->capacity *= 2;
			wins->windows = HeapReAlloc(GetProcessHeap(),
				0, wins->windows, (SIZE_T)(sizeof(HWND)*wins->capacity));
		}
	}
	wins->windows[wins->count++] = hwnd;
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
				(SIZE_T)(sizeof(HWND)*m)
			);
		}
		wins->count--;
		break;		
	}
}

static void windows_destroy(Windows *wins)
{
	HeapFree(GetProcessHeap(), 0, wins->windows);
}

static int is_valid_window(HWND hwnd)
{
	WINDOWINFO wi;
	wi.cbSize = sizeof(wi);
	GetWindowInfo(hwnd, &wi);
	return (wi.dwStyle & WS_VISIBLE) && !(wi.dwExStyle & WS_EX_TOOLWINDOW);
}

static void register_hotkey(int id, int mod, int vk)
{
	if(!RegisterHotKey(NULL, id, mod, vk)) {
		MessageBoxA(NULL, "could not register hotkey", "error",
			MB_ICONEXCLAMATION);
		ExitProcess(1);
	}
}

static BOOL enum_func(HWND hwnd, LONG_PTR lParam)
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
	EnumWindows((WNDENUMPROC)&enum_func, (LONG_PTR)v);
}

static void virgo_init(Virgo *v)
{
	#define MOD_NOREPEAT 0x4000
	int i;
	v->current = 0;
	for(i=0; i<NUM_DESKTOPS; i++) {
		v->desktops[i].windows = NULL;
		v->desktops[i].count = 0;
		v->desktops[i].capacity = 0;
		register_hotkey(i*2, MOD_ALT|MOD_NOREPEAT, i+1+0x30);
		register_hotkey(i*2+1, MOD_CONTROL|MOD_NOREPEAT, i+1+0x30);
	}
	register_hotkey(i*2, MOD_ALT|MOD_CONTROL|MOD_SHIFT|MOD_NOREPEAT, 'Q');
	trayicon_init(&v->trayicon);
}

static void virgo_deinit(Virgo *v)
{
	int i;
	for(i=0; i<NUM_DESKTOPS; i++) {
		windows_show(&v->desktops[i]);
		windows_destroy(&v->desktops[i]);
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

#ifdef RELEASE
extern int __main(void) asm("__main");
int __main(void)
#else
int main(int argc, char **argv)
#endif
{
	Virgo v;
	MSG msg;
	virgo_init(&v);
	while(GetMessageA(&msg, NULL, 0, 0)) {
		if(msg.message != WM_HOTKEY) {
			continue;
		}
		if(msg.wParam == NUM_DESKTOPS*2) {
			break;
		}
		if(msg.wParam%2 == 0) {
			virgo_go_to_desk(&v, msg.wParam/2);
		} else {
			virgo_move_to_desk(&v, (msg.wParam-1)/2);
		}
	}
	virgo_deinit(&v);
	return 0;
}
