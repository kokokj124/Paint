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
HWND hwndHinh[3], hwndMauNen[5], hwndMauVien[5], hwndGroup[4], hwndCongCu[4];
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int xRight, yBottom, xLeft, yTop, Hinh = -1, penColor = 0;
    static HBRUSH hBrush[5], hbrushHinh;
    static HPEN hPen[5], hPenHinh;
    static int listHinh[] = { 131 , 132 , 133 ,154, 155, 156, 157};
    static int hinh11 = 131;
    DWORD  color[] = { RGB(64,   0,  64),	RGB(128,   0, 128),	RGB(128,   0,  64),	RGB(128, 128, 192),	RGB(255, 128, 192),
    RGB(255, 128, 128),RGB(255, 128, 255),	RGB(255,   0, 255), RGB(255,   0, 128),	RGB(128,   0, 255), RGB(64,   0, 128),
    RGB(255, 255, 128),	RGB(255, 255,   0),	RGB(255, 128,  64),	RGB(255, 128,   0),	RGB(128,  64,   0),	RGB(128, 128,   0),
    RGB(0,   0,   0),	RGB(64,   0,   0),	RGB(128,   0,   0),	RGB(128,  64,  64),	RGB(255,   0,   0),  RGB(255, 255, 255),
    RGB(128, 128,  64),	RGB(0,  64,   0),	RGB(0, 128,   0),	RGB(0, 255,   0),	RGB(128, 255,   0),	RGB(128, 255, 128),
    RGB(0, 255, 128),	RGB(0, 255,  64),	RGB(0, 128, 128),	RGB(0, 128,  64),	RGB(0,  64,  64),	RGB(128, 128, 128),
    RGB(64, 128, 128),	RGB(0,   0, 128),	RGB(0,   0, 255),	RGB(0,  64, 128),	RGB(0, 255, 255), RGB(128, 255, 255),
    RGB(0, 128, 255),	RGB(0, 128, 192),	RGB(128, 128, 255),	RGB(0,   0, 160),	RGB(0,   0,  64),	RGB(192, 192, 192),
    RGB(64,   0,  64),};
    HDC hdc; // Bien De Lay Handle Cua Ngu Canh Thiet Bi (DC-Divice Context)
    static bool flag = false;
    switch (message)
    {
    case WM_CREATE:
    {
        hwndGroup[0] = CreateWindow(TEXT("button"), TEXT("Chọn hình"),
            WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 3, 3, 130, 80, hWnd, (HMENU)20, NULL, NULL);

        for (int i = 0; i < 3; i++) {
            hwndHinh[i] = CreateWindow(TEXT("button"), TEXT(""),
               WS_CHILD | WS_VISIBLE | BS_BITMAP, 2 + i * 40 + 8, 25, 40, 40, hWnd, (HMENU)i, NULL, NULL);
            HANDLE  hbitmap = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(listHinh[i]), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
            SendMessage(hwndHinh[i], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbitmap);
        }

        hwndGroup[1] = CreateWindow(TEXT("button"), TEXT("Chọn màu nền"),
            WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 140, 3, 300, 80, hWnd, (HMENU)21, NULL, NULL);
        hwndGroup[2] = CreateWindow(TEXT("button"), TEXT("Chọn màu đường viền"),
            WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 450, 3, 300, 80, hWnd, (HMENU)22, NULL, NULL);
        for (int i = 0; i < 5; i++)
        {
            hwndMauNen[i] = CreateWindow(TEXT("button"), TEXT(""), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 143 + i * 50 + 8 , 25, 50, 50, hWnd, HMENU(i + 3) , NULL, NULL);
            hwndMauVien[i] = CreateWindow(TEXT("button"), TEXT("") ,WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 453 + i * 50 + 8 , 25, 50, 50, hWnd, HMENU(i + 8) , NULL, NULL);
            hBrush[i] = CreateSolidBrush(color[i]);
        }

        hwndGroup[3] = CreateWindow(TEXT("button"), TEXT("Chọn bút vẽ"),
            WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 760, 3, 220, 80, hWnd, (HMENU)23, NULL, NULL);
        for (int i = 0; i < 4; i++) {
            hwndCongCu[i] = CreateWindow(TEXT("button"), TEXT(""),
                WS_CHILD | WS_VISIBLE | BS_BITMAP, 760 + i * 40 + 8, 25, 40, 40, hWnd, (HMENU)(i+13), NULL, NULL);
            HANDLE  hbitmap = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(listHinh[i + 3]), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
            SendMessage(hwndCongCu[i], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbitmap);
        }
        break;

    }

    case WM_MOUSEMOVE:
    {
        if (flag == true) {
            hdc = GetDC(hWnd);
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            for (int i = 1; i < 6; i++) {
                for (int j = 1; j < 6; j++) {
                    SetPixel(hdc, x + i, y + j,color[penColor]);
                }
            }
            ReleaseDC(hWnd, hdc);
        }
        break;
    }
    case WM_LBUTTONDOWN:
        {
            if (Hinh == 13)
            {
                if (flag == true) flag = false;
                else flag = true;
            }
            xLeft = LOWORD(lParam);
            yTop = HIWORD(lParam);
            break;
        }
    case WM_LBUTTONUP:
        {
            xRight = LOWORD(lParam);
            yBottom = HIWORD(lParam);
            hdc = GetDC(hWnd);
            SelectObject(hdc, hPenHinh);
            SelectObject(hdc, hbrushHinh);
            if(Hinh == 0) Rectangle(hdc, xLeft, yTop, xRight, yBottom);
            else if(Hinh == 1) Ellipse(hdc, xLeft, yTop, xRight, yBottom);
            else if (Hinh == 2) {
                POINT apt[] = { {xLeft, yBottom}, {xLeft, yTop}, {xRight, yBottom}, {xLeft, yBottom}, {xRight, yTop }, {xRight,yBottom}};
                Polygon(hdc, apt,6);
            }
            ReleaseDC(hWnd, hdc);
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
                InvalidateRect(hwndHinh[0], NULL, FALSE);
                break;
            case 1:
                Hinh = 1;
                InvalidateRect(hwndHinh[1], NULL, FALSE);// Khai Bao Cua So Can Ve Lai
                break;
            case 2:
                Hinh = 2;
                InvalidateRect(hwndHinh[2], NULL, FALSE);
            case 3:
                hbrushHinh = hBrush[0];
                InvalidateRect(hwndMauNen[0], NULL, FALSE);
                break;
            case 4:
                hbrushHinh = hBrush[1];
                InvalidateRect(hwndMauNen[1], NULL, FALSE);
                break;
            case 5:
                hbrushHinh = hBrush[2];
                InvalidateRect(hwndMauNen[2], NULL, FALSE);
                break;
            case 6:
                hbrushHinh = hBrush[3];
                InvalidateRect(hwndMauNen[3], NULL, FALSE);
                break;
            case 7:
                hbrushHinh = hBrush[4];
                InvalidateRect(hwndMauNen[4], NULL, FALSE);
                break;
            case 8:
                hPenHinh = CreatePen(PS_SOLID, 1, color[0]);
                penColor = 0;
                InvalidateRect(hwndMauVien[0], NULL, FALSE);
                break;
            case 9:
                hPenHinh = CreatePen(PS_SOLID, 1, color[1]);
                penColor = 1;
                InvalidateRect(hwndMauVien[1], NULL, FALSE);
                break;
            case 10:
                hPenHinh = CreatePen(PS_SOLID, 1, color[2]);
                penColor = 2;
                InvalidateRect(hwndMauVien[2], NULL, FALSE);
                break;
            case 11:
                hPenHinh = CreatePen(PS_SOLID, 1, color[3]);
                penColor = 3;
                InvalidateRect(hwndMauVien[3], NULL, FALSE);
                break;
            case 12:
                hPenHinh = CreatePen(PS_SOLID, 1, color[4]);
                penColor = 4;
                InvalidateRect(hwndMauVien[4], NULL, FALSE);
                break;
            case 13:
                Hinh = 13;
                break;
            case 14:
                break;
            case 15:
                break;
            case 16:
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
