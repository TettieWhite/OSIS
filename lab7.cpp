#include "framework.h"
#include <windows.h>
#include <tchar.h>

#define ADD_BUTTON 1
#define CLEAR_BUTTON 2
#define TO_RIGHT_BUTTON 3
#define DELETE_BUTTON 4
#define ID_EDIT 5

#define ID_LIST1   6
#define ID_LIST2   7



TCHAR g_szClassName[] = _T("myWindowClass");
const int WND_WIDTH = 720;
const int WND_HEIGHT = 400;

static HWND hTextEdt;
static HWND hLeftListBox;
static HWND hRightListBox;
static HWND hAddBttn;
static HWND hClearBttn;
static HWND hDeleteBttn;
static HWND hToRightBttn;


void InitControls(HWND hwnd) {
	hTextEdt = CreateWindowEx(
		0, L"EDIT",
		NULL,
		WS_CHILD | WS_VISIBLE | WS_VSCROLL |
		ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
		30, 30, WND_WIDTH - 60, 50,
		hwnd,
		(HMENU)ID_EDIT,
		(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
		NULL);
	hLeftListBox = CreateWindow(L"listbox", NULL,
		WS_CHILD | WS_VISIBLE | LBS_STANDARD |
		LBS_WANTKEYBOARDINPUT | LBS_MULTIPLESEL,
		50, 160, 250, 150,
		hwnd, (HMENU)ID_LIST1,
		(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
		NULL);
	hRightListBox = CreateWindow(L"listbox", NULL,
		WS_CHILD | WS_VISIBLE | LBS_STANDARD |
		LBS_WANTKEYBOARDINPUT | LBS_MULTIPLESEL,
		420, 160, 250, 150,
		hwnd, (HMENU)ID_LIST2,
		(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
		NULL);
	hToRightBttn = CreateWindow(
		L"BUTTON",
		L"-->",
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		(WND_WIDTH / 2) - 30, 200, 60, 30,
		hwnd, (HMENU)TO_RIGHT_BUTTON, NULL, NULL
	);
	hAddBttn = CreateWindow(
		L"BUTTON",
		L"ADD",
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		(WND_WIDTH / 2) - 60, 100, 120, 30,
		hwnd, (HMENU)ADD_BUTTON, NULL, NULL
	);
	hClearBttn = CreateWindow(
		L"BUTTON",
		L"CLEAR",
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		(WND_WIDTH / 2) - 30, 240, 60, 30,
		hwnd, (HMENU)CLEAR_BUTTON, NULL, NULL
	);
	hDeleteBttn = CreateWindow(
		L"BUTTON",
		L"DELETE",
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		(WND_WIDTH / 2) - 30, 280, 60, 30,
		hwnd, (HMENU)DELETE_BUTTON, NULL, NULL
	);
}

void TransferToRight(HWND hwnd) {
	int totalSelected = SendMessage(hLeftListBox, LB_GETSELCOUNT, 0, 0);
	if (totalSelected != 1) {
		MessageBox(hwnd, L"You may transfer only one item at a time", L"Item transfer", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	int* selectedItems = new int[1];
	SendMessage(hLeftListBox, LB_GETSELITEMS, (WPARAM)totalSelected, (LPARAM)selectedItems);

	int len = SendMessage(hLeftListBox, LB_GETTEXTLEN, selectedItems[0], NULL);
	TCHAR* buff = new TCHAR[len + 1];

	SendMessage(hLeftListBox, LB_GETTEXT, selectedItems[0], (LPARAM)buff);

	// if new string isn't present in the listbox 
	if (SendMessage(hRightListBox, LB_FINDSTRINGEXACT, -1, (LPARAM)buff) == LB_ERR)
		SendMessage(hRightListBox, LB_ADDSTRING, NULL, (LPARAM)buff);
	delete[] selectedItems;
	delete[] buff;
}

void FromEditToListBox(HWND hwnd) {
	int len = GetWindowTextLength(hTextEdt) + 1;
	TCHAR* buff = new TCHAR[len + 1];

	if (!GetWindowText(hTextEdt, buff, len))
		return;

	int count = 0;
	for (int i = 0; i < len - 1; i++) {
		if (buff[i] == ' ' || buff[i] == '\t')
			count++;
	}

	if (count == len - 1) {
		MessageBox(hwnd, L"String cannot consist of spaces", L"String of spaces", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	// if new string isn't present in the listbox 
	if (SendMessage(hLeftListBox, LB_FINDSTRINGEXACT, -1, (LPARAM)buff) == LB_ERR)
		SendMessage(hLeftListBox, LB_ADDSTRING, NULL, (LPARAM)buff);

	delete[] buff;
}

void DeleteSelectedItems(HWND hListBox) {
	int totalSelected = SendMessage(hListBox, LB_GETSELCOUNT, 0, 0);

	int* selectedItems = new int[totalSelected];
	SendMessage(hListBox, LB_GETSELITEMS, (WPARAM)totalSelected, (LPARAM)selectedItems);

	for (int i = totalSelected - 1; i >= 0; i--) {
		SendMessage(hListBox, LB_DELETESTRING, (WPARAM)selectedItems[i], 0);
	}
	delete[] selectedItems;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_CREATE: {
		InitControls(hwnd);
		break;
	}
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ADD_BUTTON: {
			FromEditToListBox(hwnd);
			break;
		}
		case TO_RIGHT_BUTTON: {
			TransferToRight(hwnd);
			break;
		}
		case CLEAR_BUTTON: {
			SendMessage(hLeftListBox, LB_RESETCONTENT, 0, 0);
			SendMessage(hRightListBox, LB_RESETCONTENT, 0, 0);
			break;
		}
		case DELETE_BUTTON: {
			DeleteSelectedItems(hLeftListBox);
			DeleteSelectedItems(hRightListBox);
			break;
		}
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

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
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		L"Lab 7",
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