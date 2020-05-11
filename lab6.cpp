#include "stdafx.h"
#include "lab66.h"

#include "windows.h"
#include "winuser.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
TCHAR message[] = L"HELLO WORLD!";
HMENU hMainMenu, hStrtStp;
HBRUSH brush;
HPEN pen;
HCURSOR hCursor;
const int TEXT_MOVE = 20;
void DrawTextM(HWND hWnd);
void UpdateText(HWND hWnd);
void DrawRect(HWND hWnd);

typedef struct _TEXT_INF
{
	int width;
	int height;
	int x;
	int y;
	int dx;
	int dy;
}TEXT_INF;

TEXT_INF text;
RECT rect;
RECT window;
HDC hDC;
PAINTSTRUCT ps;
HWND hMainWindow;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCMdShow)
{
	TCHAR className[] = L"windowK";

	MSG msg;
	WNDCLASSEX wc = {};

	text.dx = TEXT_MOVE;
	text.dy = TEXT_MOVE;
	rect.left = 200;
	rect.right = 290;
	rect.top = 200;
	rect.bottom = 230;
	text.width = (rect.right - rect.left);
	text.height = (rect.bottom - rect.top);


	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.lpszMenuName = L"menu";
	wc.lpszClassName = className;
	wc.cbClsExtra = NULL;
	wc.cbClsExtra = NULL;
	wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2)); 
	wc.hIconSm = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON3)); 
	wc.hCursor = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURSOR1));
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hInstance = hInstance;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"", L"", MB_OK);
		return NULL;
	}

	// creating window

	DWORD style = NULL;
	hMainWindow = CreateWindowEx(
		style,
		className,
		L"My window",
		WS_OVERLAPPEDWINDOW | WS_VSCROLL,
		CW_USEDEFAULT,
		NULL,
		CW_USEDEFAULT,
		NULL,
		(HWND)NULL,
		NULL,
		HINSTANCE(hInstance),
		NULL);

	if (!hMainWindow) {
		MessageBox(NULL, L"Error while creating!", L"Error", MB_OK);
		return NULL;
	}

	AppendMenu((hStrtStp = CreatePopupMenu()), MF_ENABLED | MF_STRING,
		ID_START, _TEXT("&Start"));
	AppendMenu(hStrtStp, MF_ENABLED | MF_STRING, ID_STOP, _TEXT("&Stop"));

	AppendMenu((hMainMenu = CreateMenu()), MF_ENABLED | MF_POPUP,
		(UINT_PTR)hStrtStp, _TEXT("&Move"));
	SetMenu(hMainWindow, hMainMenu);
	hCursor = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURSOR1));

	ShowWindow(hMainWindow, nCMdShow);
	UpdateWindow(hMainWindow);

	hDC = BeginPaint(hMainWindow, &ps);
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	EndPaint(hMainWindow, &ps);
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	COLORREF textColor = RGB(225, 0, 0);

	switch (uMsg) {
	case WM_PAINT:
		break;
	case WM_TIMER:
		GetClientRect(hWnd, &window);
		DrawRect(hWnd);
		UpdateText(hWnd);
		DrawTextM(hWnd);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_START:
			SetTimer(hMainWindow, ID_TIMER, 50, NULL);
			break;
		case ID_STOP:
			KillTimer(hWnd, ID_TIMER);
			break;
		default:
			break;
		}
		break;
	case WM_SETCURSOR:
		SetCursor(hCursor);
		return TRUE;
	case WM_DESTROY: PostQuitMessage(NULL); break;
	default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return NULL;
}

void DrawTextM(HWND hWnd)
{
	COLORREF textColor = RGB(225, 0, 0);

	SetTextColor(hDC, textColor);
	DrawText(hDC, message, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	return;
}

void DrawRect(HWND hWnd)
{
	COLORREF rectColor = RGB(255, 255, 255);

	brush = CreateSolidBrush(rectColor);
	SelectObject(hDC, brush);
	Rectangle(hDC, window.left, window.top, window.right, window.bottom);
	return;
}

void UpdateText(HWND hWnd)
{
	rect.top += text.dy;
	rect.left += text.dx;
	rect.right += text.dx;
	rect.bottom += text.dy;

	if (rect.left < 0)
	{
		rect.left = 0;
		text.dx = TEXT_MOVE;
	}
	else if (rect.left + text.width > (window.right))
	{
		rect.left = (window.right) - text.width;
		text.dx = -TEXT_MOVE;
	}

	if (rect.top < 0)
	{
		rect.top = 0;
		text.dy = TEXT_MOVE;
	}
	else if (rect.top + text.height > (window.bottom))
	{
		rect.top = (window.bottom) - text.height;
		rect.bottom = (window.bottom);
		text.dy = -TEXT_MOVE;
	}
}
