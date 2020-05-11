#include "framework.h"
#include "lab9.h"

#define MAX_LOADSTRING 100

#define TIMER_ID 5
#define TILE_SIZE 32

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

HBITMAP image;
int currentX;
int currentY;
int goalX;
int goalY;
int currentImageIndex = -1;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

void updateImage(HDC hdc);
void updatePosition();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LAB9, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
		return FALSE;

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB9));

	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB9));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB9);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
		return FALSE;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		image = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		currentX = 0;
		currentY = 0;
		goalX = 0;
		goalY = 0;
		SetTimer(hWnd, TIMER_ID, 50, NULL);
		break;
	case WM_LBUTTONDOWN:
		goalX = LOWORD(lParam);
		goalY = HIWORD(lParam);
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId) {
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		updateImage(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hWnd, TIMER_ID);
		DeleteObject(image);
		break;
	case WM_TIMER:
		InvalidateRect(hWnd, NULL, true);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void updatePosition() {

	double diffX = goalX - currentX;
	double diffY = goalY - currentY;
	if (diffX < 5 && diffX > -5) {
		currentX += diffX;
	}
	else {
		if (diffX > 0) {
			currentX += 5;
		}
		else {
			currentX -= 5;
		}
	}
	if (diffY < 5 && diffY > -5) {
		currentY += diffY;
	}
	else {
		if (diffY > 0) {
			currentY += 5;
		}
		else {
			currentY -= 5;
		}
	}
}

void updateImage(HDC hdc) {
	if (++currentImageIndex == 3) {
		currentImageIndex = 0;
	}

	HDC memory = CreateCompatibleDC(hdc);
	HGDIOBJ hOldBitmap = SelectObject(memory, image);

	if (currentX != goalX || currentY != goalY) {
		updatePosition();
	}

	StretchBlt(hdc, currentX, currentY, TILE_SIZE, TILE_SIZE, memory,
		currentImageIndex * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE, SRCCOPY);
	DeleteDC(memory);
}