#include "stdafx.h"
#include "lab11.h"

#define MAX_LOADSTRING 100

#define ID_BTN_START 101
#define ID_BTN_STOP 102

#define TIMER_ID 121
#define TIMER_INTERVAL 100

HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            

HANDLE thread1;
HANDLE thread2;
HANDLE thread3;

bool threadStopped = false;
bool isWorking = false;

int rect1Lenght = 0;
int rect2Lenght = 0;
int rect3Lenght = 0;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void createControls(HWND hWnd);
void draw(HDC hdc);
void initThreads();
void startThreads();
DWORD WINAPI thread1Proc(CONST LPVOID lpParam);
DWORD WINAPI thread2Proc(CONST LPVOID lpParam);
DWORD WINAPI thread3Proc(CONST LPVOID lpParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB11, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB11));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB11));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB11);
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
		createControls(hWnd);
		initThreads();
		SetTimer(hWnd, TIMER_ID, TIMER_INTERVAL, NULL);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
			case ID_BTN_START:
				isWorking = true;
				break;
			case ID_BTN_STOP:
				isWorking = false;
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
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
			draw(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_TIMER:
		if (isWorking) {
			startThreads();
			InvalidateRect(hWnd, NULL, true);
		}
		break;
    case WM_DESTROY:
		threadStopped = true;
		startThreads();
		KillTimer(hWnd, TIMER_ID);
		PostQuitMessage(0);
		CloseHandle(thread1);
		CloseHandle(thread2);
		CloseHandle(thread3);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void createControls(HWND hWnd) {
	CreateWindow(L"button", L"Start",
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		10, 30,
		95, 35,
		hWnd, (HMENU)ID_BTN_START,
		hInst, NULL);
	CreateWindow(L"button", L"Stop",
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		10, 90,
		95, 35,
		hWnd, (HMENU)ID_BTN_STOP,
		hInst, NULL);
}

void initThreads() {
	thread1 = CreateThread(NULL, 0, &thread1Proc, NULL, 0, NULL);
	thread2 = CreateThread(NULL, 0, &thread2Proc, NULL, 0, NULL);
	thread3 = CreateThread(NULL, 0, &thread3Proc, NULL, 0, NULL);
}

void startThreads() {
	ResumeThread(thread1);
	ResumeThread(thread2);
	ResumeThread(thread3);
}

DWORD WINAPI thread1Proc(CONST LPVOID lpParam) {
	while (!threadStopped) {
		rect1Lenght += 2;	
		SuspendThread(GetCurrentThread());
	}
	return 0;
}

DWORD WINAPI thread2Proc(CONST LPVOID lpParam) {
	while (!threadStopped) {
		rect2Lenght += 1;
		SuspendThread(GetCurrentThread());
	}
	return 0;
}

DWORD WINAPI thread3Proc(CONST LPVOID lpParam) {
	while (!threadStopped) {
		rect3Lenght += 3;
		SuspendThread(GetCurrentThread());
	}
	return 0;
}

void draw(HDC hdc) {
	HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(hdc, brush);
	Rectangle(hdc, 150, 50, 150+rect1Lenght, 150);
	brush = CreateSolidBrush(RGB(0, 255, 0));
	SelectObject(hdc, brush);
	Rectangle(hdc, 150, 200, 150+rect2Lenght, 300);
	brush = CreateSolidBrush(RGB(0, 0, 255));
	SelectObject(hdc, brush); 
	Rectangle(hdc, 150, 350, 150+rect3Lenght, 450);
}