#include "stdafx.h"
#include "lab8.h"
#include <windows.h>
#include <tchar.h>



const int WND_WIDTH = 700;
const int WND_HEIGHT = 500;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void DrawTree(const HWND& hwnd, const int X, const int Y);
void DrawPctr();
void line(HDC& hdc, const int x1, const int y1, const int x2, const int y2);
void DrawBitmap(HWND hwnd, int x, int y, HBITMAP image);
void DrawHouse(const HWND& hwnd, const int X, const int Y);
void DrawBackground(const HWND& hwnd);
void ClearWorkspace(HWND hwnd);


WNDCLASSEX wc;
HWND hwnd;
MSG Msg;
HWND hDrawButton;
HWND hClearButton;
HBITMAP hDrawBitmap;
HBITMAP hClearBitmap;
HDC hdc;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	hDrawBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1));
	hClearBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP2));
	

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"myClass";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"myClass",
		L"Lab 8",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, WND_WIDTH, WND_HEIGHT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&Msg, NULL, 0, 0) > 0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return Msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_CREATE: {
		RECT WinCoord = {};
		GetWindowRect(hwnd, &WinCoord);
		ClearWorkspace(hwnd);

		hDrawButton = CreateWindowEx(
			NULL,
			L"BUTTON",
			L"DRAW",
			WS_VISIBLE | WS_CHILD| BS_OWNERDRAW | WS_BORDER,
			WND_WIDTH / 2 - 150, WND_HEIGHT - 130, 72, 72, // correct
			hwnd, (HMENU)DRAW_BUTTON,
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL);

		hClearButton = CreateWindowEx(
			NULL,
			L"BUTTON",
			L"CLEAR",
			WS_VISIBLE | WS_CHILD| BS_OWNERDRAW | WS_BORDER,
			WND_WIDTH / 2 + 50, WND_HEIGHT - 130, 72, 72, // correct
			hwnd, (HMENU)CLEAR_BUTTON,
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL);
		break;
	}
	case WM_DRAWITEM: {

		HDC hCompDC;
		BITMAP bm;
		HBRUSH hCurBrush;
		LPDRAWITEMSTRUCT pdis;
		int x0, y0 = 0;

		pdis = (LPDRAWITEMSTRUCT)lParam;
		hCompDC = CreateCompatibleDC(pdis->hDC);
		
		switch (pdis->CtlID)
		{
		case DRAW_BUTTON: {
			SelectObject(hCompDC, hDrawBitmap);
			break;
		}
		case CLEAR_BUTTON: {
			SelectObject(hCompDC, hClearBitmap);
		}
		default:
			break;
		}
		
		GetObject(hDrawBitmap, sizeof(bm), &bm);

		x0 = ((pdis->rcItem.right - pdis->rcItem.left) - bm.bmWidth) / 2;
		
		BitBlt(pdis->hDC, x0, y0, bm.bmWidth, bm.bmHeight, hCompDC, 0, 0, SRCCOPY);
		
		HBRUSH frame = CreateSolidBrush(RGB(0, 255, 0));
		FrameRect(pdis->hDC, &pdis->rcItem, frame);
		DeleteDC(hCompDC);

		break;
	}
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
			case DRAW_BUTTON: {
				hdc = GetDC(hwnd);
				DrawPctr();
				DeleteDC(hdc);
				break;
			}
			case CLEAR_BUTTON: {
				ClearWorkspace(hwnd);
				break;
			}
		default: break;
		}
		break;
	}
	case WM_CLOSE: {
		DestroyWindow(hwnd);
		break;
	}
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void ClearWorkspace(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	InvalidateRect(hwnd, &rect, true);
}

void DrawBitmap(HWND hwnd, int x, int y, HBITMAP image)
{
	BITMAP bm;
	PAINTSTRUCT ps;
	HDC hdc, hdcMem;

	hdc = BeginPaint(hwnd, &ps);
	hdcMem = CreateCompatibleDC(hdc);

	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, image);

	GetObject(image, sizeof(bm), &bm);

	BitBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);

	EndPaint(hwnd, &ps);
}

void DrawPctr()
{
	DrawBackground(hwnd);
	DrawTree(hwnd, 100, 260);
	DrawHouse(hwnd, 190, 0);
	DrawTree(hwnd, 560, 320);
}

void line(HDC& hdc, const int x1, const int y1, const int x2, const int y2) {
	MoveToEx(hdc, x1, y1, 0);
	LineTo(hdc, x2, y2);
}

void DrawTree(const HWND& hwnd, const int X, const int Y) {
	
	HBRUSH brush = CreateSolidBrush(RGB(3, 243, 159));
	SelectObject(hdc, brush);
	line(hdc, X, Y, X, Y - 150);

	line(hdc, X, Y - 75, X - 50, Y - 75 - 15);

	Ellipse(hdc, X - 70, Y - 105, X - 40, Y - 105 + 30);

	line(hdc, X, Y - 95, X - 50, Y - 95 - 15);
	Ellipse(hdc, X - 75, Y - 120, X - 50, Y - 120 + 15);
	Ellipse(hdc, X - 50, Y - 130, X - 35, Y - 120 + 15);

	Ellipse(hdc, X - 10, Y - 150, X + 10, Y - 150 + 15);

	line(hdc, X, Y - 60, X + 80, Y - 165);
	line(hdc, X, Y - 90, X + 30, Y - 135);
	line(hdc, X + 25, Y - 95, X + 85, Y - 100);
	line(hdc, X + 50, Y - 130, X + 55, Y - 170);

	Ellipse(hdc, 15 + X + 55, 15 + Y - 170, 15 + X + 75, 15 + Y - 190);
	Ellipse(hdc, 15 + X + 34, 15 + Y - 180, 15 + X + 46, 15 + Y - 200);
	Ellipse(hdc, 25 + X + (-5), 25 + Y - 120, 25 + X + 7, 25 + Y - 144);
	Ellipse(hdc, 15 + X + 55, 15 + Y - 103, 15 + X + 75, 15 + Y - 127);
	Ellipse(hdc, 15 + X + 10, 15 + Y - 140, 15 + X + 28, 15 + Y - 158);

	DeleteObject(brush);
}

void DrawHouse(const HWND& hwnd, const int X, const int Y) {
	
	/*ФАСАД ДОМА*/
	HBRUSH  brush = CreateSolidBrush(RGB(158, 191, 202));
	SelectObject(hdc, brush);
	Rectangle(hdc, X, Y + 160, X + 130, Y + 300);
	DeleteObject(brush);

	/*ТРЕУГОЛЬНАЯ КРЫША*/
	POINT poly[3] = { {X,Y + 160 },{ X + 130 , Y + 160 },{X + 130 / 2, Y + 100} };
	brush = CreateSolidBrush(RGB(178, 31, 66));
	SelectObject(hdc, brush);
	Polygon(hdc, poly, 3);
	DeleteObject(brush);

	/*БОК КРЫШИ ПАРАЛЛЕЛОГРАММ*/
	POINT poly2[4] = { {X + 65,Y + 100 },{X + 280 , Y + 100 },{X + 345,Y + 160}, {X + 130, Y + 160 } };
	brush = CreateSolidBrush(RGB(169, 33, 57));
	SelectObject(hdc, brush);
	Polygon(hdc, poly2, 4);
	DeleteObject(brush);

	/*БОК ДОМА*/
	brush = CreateHatchBrush(7, RGB(132, 160, 169));
	SelectObject(hdc, brush);
	Rectangle(hdc, X + 345, Y + 160, X + 130, Y + 300);
	DeleteObject(brush);

	/*ДВЕРЬ*/
	brush = CreateSolidBrush(RGB(168, 106, 58));
	SelectObject(hdc, brush);
	Rectangle(hdc, X + 70, Y + 220, X + 110, Y + 300);
	DeleteObject(brush);

	/*ОКНА*/
	brush = CreateSolidBrush(RGB(107, 234, 234));	
	SelectObject(hdc, brush);
	Rectangle(hdc, X + 150, Y + 200, X + 190, Y + 240);
	Rectangle(hdc, X + 220, Y + 200, X + 260, Y + 240);
	Rectangle(hdc, X + 290, Y + 200, X + 330, Y + 240);
	DeleteObject(brush);
}

void DrawBackground(const HWND& hwnd)
{
	HBRUSH brush = CreateSolidBrush(RGB(62, 184, 255));
	SelectObject(hdc, brush);
	Rectangle(hdc, 0, 0, WND_WIDTH, WND_HEIGHT - 280); //sky
	DeleteObject(brush);

	brush = CreateSolidBrush(RGB(95, 255, 111));
	SelectObject(hdc, brush);
	Rectangle(hdc, 0, WND_HEIGHT - 280, WND_WIDTH, WND_HEIGHT - 150); //ground
	DeleteObject(brush);
}