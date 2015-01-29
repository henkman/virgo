#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include "stretchy_buffer.h"
#include "tray.h"

#define NUM_DESKTOPS 4

typedef struct {
	HWND *windows;
	int count;
} Windows;

static int current;
static Windows desktops[NUM_DESKTOPS];

static void windows_mod(Windows *wins, int state)
{
	int i;
	for(i = 0; i < wins->count; i++) {
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
	int i, o = -1;
	for(i = 0; i < wins->count; i++) {
		if(wins->windows[i] == hwnd) {
			o = i;
			break;
		}
	}
	if(o == -1) {
		return;
	}
	for(i = o; i < (wins->count - 1); i++) {
		wins->windows[i] = wins->windows[i + 1];
	}
	wins->count--;
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
	Windows *wins;
	if(!is_valid_window(hwnd)) {
		return 1;
	}
	wins = (Windows *) lParam;
	for(i = 0; i < wins->count; i++) {
		if(wins->windows[i] == hwnd) {
			return 1;
		}
	}
	windows_add(&desktops[current], hwnd);

	return 1;
}

static void update_windows()
{
	int i, e;
	HWND hwnd;
	Windows *desk;
	Windows wins;
	wins.windows = NULL;
	wins.count = 0;
	for(i = 0; i < NUM_DESKTOPS; i++) {
		desk = &desktops[i];
		for(e = 0; e < desk->count; e++) {
			hwnd = desk->windows[e];
			if(GetWindowThreadProcessId(hwnd, NULL) == 0) {
				windows_del(desk, hwnd);
			} else {
				windows_add(&wins, hwnd);
			}
		}
	}
	EnumWindows((WNDENUMPROC)&enum_func, (LPARAM)&wins);
	sb_free(wins.windows);
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
		desktops[i].windows = NULL;
		desktops[i].count = 0;
		register_hotkey(i * 2, MOD_ALT|MOD_NOREPEAT, i + 1 + 0x30);
		register_hotkey(i * 2  + 1, MOD_CONTROL|MOD_NOREPEAT, i + 1 + 0x30);
	}
	register_hotkey(i * 2, MOD_ALT|MOD_CONTROL|MOD_SHIFT|MOD_NOREPEAT, 'Q');
	trayicon_create();
}

static void cleanup()
{
	int i;
	for(i = 0; i < NUM_DESKTOPS; i++) {
		windows_show(&desktops[i]);
		sb_free(desktops[i].windows);
	}
	trayicon_remove();
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
	windows_del(&desktops[current], hwnd);
	windows_add(&desktops[desk], hwnd);
	ShowWindow(hwnd, SW_HIDE);
}

static void move_to_desk(int desk)
{
	if(current == desk) {
		return;
	}
	update_windows();
	windows_hide(&desktops[current]);
	windows_show(&desktops[desk]);
	current = desk;
	trayicon_set(desk + 1);
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
