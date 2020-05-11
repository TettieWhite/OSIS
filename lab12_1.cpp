#include "stdafx.h"
#include "lab12.h"

#define MAX_LOADSTRING 100

#define TIMER_ID 121
#define TIMER_INTERVAL 100

HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            

HANDLE thread1;
HANDLE thread2;
HANDLE thread3;
CRITICAL_SECTION cs;

int margin = 0;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

void line(HDC hdc, int x1, int y1, int x2, int y2);
void circle(HDC hdc, int x, int y, int r);
void drawRect(HDC hdc);

DWORD WINAPI threadProc(CONST LPVOID lpParam);
DWORD WINAPI threadProc2(CONST LPVOID lpParam);
void initThreads();
void startThreads();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB12, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB12));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB12));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB12);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		InitializeCriticalSection(&cs);
		initThreads();
		SetTimer(hWnd, TIMER_ID, TIMER_INTERVAL, NULL);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			drawRect(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_TIMER:
		InvalidateRect(hWnd, NULL, TRUE);
		break;
    case WM_DESTROY:
		CloseHandle(thread1);
		CloseHandle(thread2);
		CloseHandle(thread3);
		DeleteCriticalSection(&cs);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void drawRect(HDC hdc) {
	Rectangle(hdc, 50, 50 + margin, 150, 150 + margin);
}

void line(HDC hdc, int x1, int y1, int x2, int y2) {
	MoveToEx(hdc, x1, y1, nullptr);
	LineTo(hdc, x2, y2);
}

void circle(HDC hdc, int x, int y, int r) {
	Ellipse(hdc, x - r, y - r, x + r, y + r);
}

void initThreads() {
	thread1 = CreateThread(NULL, 0, &threadProc, NULL, 0, NULL);
	thread2 = CreateThread(NULL, 0, &threadProc2, NULL, 0, NULL);
}

void startThreads() {
	ResumeThread(thread1);
	ResumeThread(thread2);
	ResumeThread(thread3);
}

DWORD WINAPI threadProc(CONST LPVOID lpParam) {
	while (true) {
		EnterCriticalSection(&cs);
		margin += 25;
		Sleep(250);
		LeaveCriticalSection(&cs);
		Sleep(250);
	}
	return 0;
}

DWORD WINAPI threadProc2(CONST LPVOID lpParam) {
	while (true) {
		EnterCriticalSection(&cs);
		margin -= 15;
		Sleep(250);
		LeaveCriticalSection(&cs);
		Sleep(250);
	}
	return 0;
}