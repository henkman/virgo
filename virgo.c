#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include "tray.h"

#define NUM_DESKTOPS 4
#define MAX_WIN_PER_DESKTOP 128

typedef struct desktop_t {
	HWND windows[MAX_WIN_PER_DESKTOP];
	int wincount;
} desktop_t;

typedef struct windows_t {
	HWND windows[MAX_WIN_PER_DESKTOP *NUM_DESKTOPS];
	int wincount;
} windows_t;

static int current;
static desktop_t desktops[NUM_DESKTOPS];

static void desktop_mod(desktop_t *desk, int state)
{
	int i;
	for(i = 0; i < desk->wincount; i++) {
		ShowWindow(desk->windows[i], state);
	}
}

static void desktop_show(desktop_t *desk)
{
	desktop_mod(desk, SW_SHOW);
}

static void desktop_hide(desktop_t *desk)
{
	desktop_mod(desk, SW_HIDE);
}

static void desktop_add(desktop_t *desk, HWND hwnd)
{
	if(desk->wincount + 1 >= MAX_WIN_PER_DESKTOP) {
		return;
	}
	desk->windows[desk->wincount++] = hwnd;
}

static void desktop_del(desktop_t *desk, HWND hwnd)
{
	int i, o = -1;
	for(i = 0; i < desk->wincount; i++) {
		if(desk->windows[i] == hwnd) {
			o = i;
			break;
		}
	}
	if(o == -1) {
		return;
	}
	for(i = o; i < (desk->wincount - 1); i++) {
		desk->windows[i] = desk->windows[i + 1];
	}
	desk->wincount--;
}

static int is_valid_window(HWND hwnd)
{
	WINDOWINFO wi;
	wi.cbSize = sizeof(wi);
	GetWindowInfo(hwnd, &wi);
	return (wi.dwStyle & WS_VISIBLE) && !(wi.dwExStyle & WS_EX_TOOLWINDOW);
}

static BOOL enum_func(HWND hwnd, LPARAM lParam)
{
	int i;
	windows_t *wins;
	if(!is_valid_window(hwnd)) {
		return 1;
	}
	wins = (windows_t *) lParam;
	for(i = 0; i < wins->wincount; i++) {
		if(wins->windows[i] == hwnd) {
			return 1;
		}
	}
	desktop_add(&desktops[current], hwnd);

	return 1;
}

static void update_windows()
{
	int i, e;
	HWND hwnd;
	windows_t wins;
	desktop_t *desk;
	wins.wincount = 0;
	for(i = 0; i < NUM_DESKTOPS; i++) {
		desk = &desktops[i];
		for(e = 0; e < desk->wincount; e++) {
			hwnd = desk->windows[e];
			if(GetWindowThreadProcessId(hwnd, NULL) == 0) {
				desktop_del(desk, hwnd);
			} else {
				wins.windows[wins.wincount++] = hwnd;
			}
		}
	}
	EnumWindows((WNDENUMPROC)&enum_func, (LPARAM)&wins);
}

static void register_hotkey(int id, int mod, int vk)
{
	if(RegisterHotKey(NULL, id, mod, vk) == 0) {
		fprintf(stderr, "could not register key\n");
		exit(1);
	}
}

static UINT MOD_NOREPEAT = 0x4000;
static void init()
{
	int i;
	for(i = 0; i < NUM_DESKTOPS; i++) {
		desktops[i].wincount = 0;
		register_hotkey(i * 2, MOD_ALT | MOD_NOREPEAT, i + 1 + 0x30);
		register_hotkey(i * 2  + 1, MOD_CONTROL | MOD_NOREPEAT, i + 1 + 0x30);
	}
	register_hotkey(i * 2, MOD_ALT | MOD_CONTROL | MOD_SHIFT | MOD_NOREPEAT, 'Q');
	createTrayIcon();
}

static void cleanup()
{
	int i;
	for(i = 0; i < NUM_DESKTOPS; i++) {
		desktop_show(&desktops[i]);
	}
	removeTrayIcon();
}

static void move_win_to_desk(int desk)
{
	HWND hwnd;

	if(current == desk) {
		return;
	}
	update_windows();
	hwnd = GetForegroundWindow();
	if(hwnd == NULL || !is_valid_window(hwnd)) {
		return;
	}
	desktop_del(&desktops[current], hwnd);
	desktop_add(&desktops[desk], hwnd);
	ShowWindow(hwnd, SW_HIDE);
}

static void move_to_desk(int desk)
{
	if(current == desk) {
		return;
	}
	update_windows();
	desktop_hide(&desktops[current]);
	desktop_show(&desktops[desk]);
	current = desk;
	setTrayIcon(desk + 1);
}

int main(int argc, char **argv)
{
	MSG msg;

	atexit(&cleanup);
	init();
	while(GetMessage(&msg, NULL, 0, 0) != 0) {
		if(msg.message != WM_HOTKEY) {
			continue;
		}
		if(msg.wParam == NUM_DESKTOPS * 2) {
			break;
		}
		if(msg.wParam % 2 == 0) {
			move_to_desk(msg.wParam / 2);
		} else {
			move_win_to_desk((msg.wParam - 1) / 2);
		}
	}
	return EXIT_SUCCESS;
}
