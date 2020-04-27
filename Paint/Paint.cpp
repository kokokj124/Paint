// Paint.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Paint.h"

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
    LoadStringW(hInstance, IDC_PAINT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAINT));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAINT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PAINT);
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
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
HWND hwndHinh[2], hwndMauNen[5], hwndMauVien[5], hwndGroup[3];
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int xRight, yBottom, xLeft, yTop, Hinh;
    static HBRUSH hBrush[5], hbrushHinh;
    static HPEN hPen[5], hPenHinh;
    COLORREF color[5] = { RGB(255,0,0),RGB(0,255,0),RGB(0,0,255),RGB(255,255,0),RGB(0,255,255) };
    HDC hdc; // Bien De Lay Handle Cua Ngu Canh Thiet Bi (DC-Divice Context)

    switch (message)
    {
    case WM_CREATE:
    {
        hwndGroup[0] = CreateWindow(TEXT("button"), TEXT("Chọn hình"),
            WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 3, 3, 130, 80, hWnd, (HMENU)20, NULL, NULL);

        for (int i = 0; i < 2; i++) {
            hwndHinh[i] = CreateWindow(TEXT("button"), TEXT(""),
                WS_CHILD | WS_VISIBLE | BS_BITMAP, 5 + i * 50 + 8, 25, 50, 50, hWnd, (HMENU)i, NULL, NULL);
            HANDLE  hbitmap = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(131), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
            SendMessage(hwndHinh[i], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbitmap); 
        }

        hwndGroup[1] = CreateWindow(TEXT("button"), TEXT("Chọn màu nền"),
            WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 140, 3, 300, 80, hWnd, (HMENU)21, NULL, NULL);
        hwndGroup[2] = CreateWindow(TEXT("button"), TEXT("Chọn màu đường viền"),
            WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 450, 3, 300, 80, hWnd, (HMENU)22, NULL, NULL);
        for (int i = 0; i < 5; i++)
        {
            hwndMauNen[i] = CreateWindow(TEXT("button"), TEXT(""),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 143 + i * 50 + 8, 25, 50, 50, hWnd, (HMENU)(i + 2), NULL, NULL);
            hwndMauVien[i] = CreateWindow(TEXT("button"), TEXT(""),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 453 + i * 50 + 8, 25, 50, 50, hWnd, (HMENU)(i + 7), NULL, NULL);
            hBrush[i] = CreateSolidBrush(color[i]);
        }
        break;

    }
    case WM_LBUTTONDOWN:
        {
            xLeft = LOWORD(lParam);
            yTop = HIWORD(lParam);
            break;
        }
    case WM_LBUTTONUP:
        {
            xRight = LOWORD(lParam);
            yBottom = HIWORD(lParam);
            hdc = GetDC(hWnd);
            break;
        }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case 0:
                Hinh = 0;
                //InvalidateRect(hwndHinh[0], NULL, FALSE);
                break;
            case 1:
                Hinh = 1;
                InvalidateRect(hwndHinh[1], NULL, FALSE);// Khai Bao Cua So Can Ve Lai
                break;
            case 2:
                hbrushHinh = hBrush[0];
                InvalidateRect(hwndMauNen[0], NULL, FALSE);
                break;
            case 3:
                hbrushHinh = hBrush[1];
                InvalidateRect(hwndMauNen[1], NULL, FALSE);
                break;
                //Tương tự cho 4,5,6
            case 7:
                hPenHinh = CreatePen(PS_SOLID, 1, color[0]);
                InvalidateRect(hwndMauVien[0], NULL, FALSE);
                break;
            case 8:
                hPenHinh = CreatePen(PS_SOLID, 1, color[1]);
                InvalidateRect(hwndMauVien[1], NULL, FALSE);
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
            break;
        }
    case WM_PAINT:
        {
            HDC hdc;
            PAINTSTRUCT ps;
            for (int i = 0; i < 5; i++)
            {
                //Lay Handel Cua Divice Context Cach1
                hdc = BeginPaint(hwndMauNen[i], &ps);
                SelectObject(hdc, GetStockObject(NULL_PEN));
                SelectObject(hdc, hBrush[i]);
                Rectangle(hdc, 0, 0, 50, 50);
                EndPaint(hwndMauNen[i], &ps);
                //Lay Handel Cua Divice Context Cach2
                hdc = GetDC(hwndMauVien[i]);
                SelectObject(hdc, GetStockObject(NULL_PEN));
                SelectObject(hdc, hBrush[i]);
                Rectangle(hdc, 0, 0, 50, 50);
                ReleaseDC(hwndMauVien[i], hdc);
            }    
            break;
        }
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
