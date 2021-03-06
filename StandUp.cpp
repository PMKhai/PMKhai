// StandUp.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "StandUp.h"
#include <windowsx.h>


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void DecreaseSeconds(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void AdjustingString(WCHAR str);
const unsigned IDT_TIMER1 = 100, IDT_TIMER2 = 200;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_STANDUP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_STANDUP));

    MSG msg;

    // Main message loop:
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



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STANDUP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_STANDUP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      450, 100, 400, 600, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;	
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
	HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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

HWND lblSeconds;
HWND lblMinutes;
HWND btnStart;
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	// Lấy font hệ thống
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	btnStart = CreateWindowEx(NULL, L"BUTTON", L"Bắt đầu",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 150, 280, 100, 40, hwnd,
		(HMENU)IDC_BTNSTART, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnStart, hFont, TRUE);

	lblMinutes = CreateWindowEx(NULL, L"STATIC", L"02", WS_CHILD | WS_VISIBLE | SS_CENTER,
		140, 100, 50, 50, hwnd,
		(HMENU)IDC_MINUTES, lpCreateStruct->hInstance, NULL);
	SetWindowFont(lblMinutes, hFont, TRUE);

	HWND lblColon = CreateWindowEx(NULL, L"STATIC", L":", WS_CHILD | WS_VISIBLE | SS_CENTER,
		190, 100, 10, 50, hwnd,
		NULL, lpCreateStruct->hInstance, NULL);
	SetWindowFont(lblColon, hFont, TRUE);

	lblSeconds = CreateWindowEx(NULL, L"STATIC", L"00", WS_CHILD | WS_VISIBLE | SS_CENTER,
		200, 100, 50, 50, hwnd,
		(HMENU)IDC_SECONDS, lpCreateStruct->hInstance, NULL);
	SetWindowFont(lblSeconds, hFont, TRUE);

	return TRUE;
}
bool status = false; // Trạng thái của đồng hồ (false: dừng, true: đang chạy)
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDC_BTNSTART:
		//MessageBox(0, 0, 0, 0); // Quick debug
		
		if (status == false) {
		SetWindowText(btnStart, L"Dừng");
		SetTimer(hwnd, IDT_TIMER1,
			1000,				            // 1 second                 
			(TIMERPROC)DecreaseSeconds);   // Timer callback   
		status = true;
		break;}
		else{
			SetWindowText(btnStart, L"Bắt đầu");
			status = false;
			break;
		}

	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hwnd);
		break;
	}
}

int seconds = 0;
int minutes = 2;
void DecreaseSeconds(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	WCHAR bufferSeconds[3];
	WCHAR bufferMinutes[3];
	if (status == true)
	{
		if (seconds == 0 && minutes != 0) {
			minutes--; // Giảm đi một phút
			seconds = 60;
			if (minutes < 10)
				wsprintf(bufferMinutes, L"0%d", minutes);
			else
				wsprintf(bufferMinutes, L"0%d", minutes);
			SetWindowText(lblMinutes, bufferMinutes); // Set text mới
		}
		seconds--; // Giảm đi một giây
		if (seconds < 10)
			wsprintf(bufferSeconds, L"0%d", seconds);
		else
			wsprintf(bufferSeconds, L"%d", seconds);
		SetWindowText(lblSeconds, bufferSeconds); // Set text mới

		if (seconds == 0 && minutes == 0) { // Nếu hết giờ
			KillTimer(hwnd, IDT_TIMER1); // Hủy bỏ timer
			MessageBox(hwnd, L"Stand up! Please...", L"Warning...", 0);
			seconds = 0;
			wsprintf(bufferSeconds, L"0%d", seconds);
			SetWindowText(lblSeconds, bufferSeconds); // Set text mới cho giây
			minutes = 2;
			wsprintf(bufferMinutes, L"0%d", minutes);
			SetWindowText(lblMinutes, bufferMinutes); // Set text mới cho phút
			SetWindowText(btnStart, L"Bắt đầu");
			status = false;
		}
	}
}