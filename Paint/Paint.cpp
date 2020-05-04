// Paint.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Paint.h"
#include <queue>
#include <map>
#include <set>
using namespace std;
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
INT_PTR CALLBACK DialogFont(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DialogPaint(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

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
	if (!InitInstance(hInstance, nCmdShow))
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

	return (int)msg.wParam;

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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAINT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PAINT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
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
static HWND hwndHinh[3], hwndMauNen[5], hwndGroup[3], hwndCongCu[6], hwndEdit = NULL;
static HPEN hPenHinh;
static LOGPEN lgpen; // khai báo struct cua pen và thiết lập các thuộc tính
static bool mauVien = false;
static 	HFONT hFont;
static 	HDC hdc; // Bien De Lay Handle Cua Ngu Canh Thiet Bi (DC-Divice Context)
static PAINTSTRUCT ps;
static COLORREF colorToMau;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int xRight, yBottom, xLeft, yTop, txtXX, txtYY, Hinh = -1, penColor = 0, x, y;
	static HBRUSH hBrush[5], hbrushHinh;
	static int listHinh[] = { 131 , 132 , 133 ,154, 155, 156, 157, 159, 163 ,164, 165, 166, 167, 168};
	static int hinh11 = 131;
	static PAINTSTRUCT ps;
	static RECT clientRect;
	static TEXTMETRIC textMetric;
	static HRGN bgRgn;
	DWORD  color[] = { RGB(255,   0,  0),	RGB(255,   255, 51),	RGB(143,   0,  109),	RGB(0, 0, 204),	RGB(0, 0, 0),
	RGB(255, 128, 128),RGB(255, 128, 255),	RGB(255,   0, 255), RGB(255,   0, 128),	RGB(128,   0, 255), RGB(64,   0, 128),
	RGB(255, 255, 128),	RGB(255, 255,   0),	RGB(255, 128,  64),	RGB(255, 128,   0),	RGB(128,  64,   0),	RGB(128, 128,   0),
	RGB(0,   0,   0),	RGB(64,   0,   0),	RGB(128,   0,   0),	RGB(128,  64,  64),	RGB(255,   0,   0),  RGB(255, 255, 255),
	RGB(128, 128,  64),	RGB(0,  64,   0),	RGB(0, 128,   0),	RGB(0, 255,   0),	RGB(128, 255,   0),	RGB(128, 255, 128),
	RGB(0, 255, 128),	RGB(0, 255,  64),	RGB(0, 128, 128),	RGB(0, 128,  64),	RGB(0,  64,  64),	RGB(128, 128, 128),
	RGB(64, 128, 128),	RGB(0,   0, 128),	RGB(0,   0, 255),	RGB(0,  64, 128),	RGB(0, 255, 255), RGB(128, 255, 255),
	RGB(0, 128, 255),	RGB(0, 128, 192),	RGB(128, 128, 255),	RGB(0,   0, 160),	RGB(0,   0,  64),	RGB(192, 192, 192),
	RGB(64,   0,  64), };
	static bool flag = false;
	static bool flag2 = false;
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
			WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 140, 3, 275, 80, hWnd, (HMENU)21, NULL, NULL);

		for (int i = 0; i < 5; i++)
		{
			//hwndMauNen[i] = CreateWindow(TEXT("button"), TEXT(""), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 143 + i * 50 + 8, 25, 50, 50, hWnd, HMENU(i + 3), NULL, NULL);

			hwndMauNen[i] = CreateWindow(TEXT("button"), TEXT(""), WS_CHILD | WS_VISIBLE | BS_BITMAP, 143 + i * 50 + 8, 25, 50, 50, hWnd, (HMENU)(i + 3), NULL, NULL);
			HANDLE  hbitmap = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(listHinh[i + 9]), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
			hBrush[i] = CreateSolidBrush(color[i]);

			SendMessage(hwndMauNen[i], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbitmap);
		}

		hwndGroup[2] = CreateWindow(TEXT("button"), TEXT("Chọn bút vẽ"),
			WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 450, 3, 260, 80, hWnd, (HMENU)23, NULL, NULL);

		for (int i = 0; i < 6; i++) {
			hwndCongCu[i] = CreateWindow(TEXT("button"), TEXT(""), WS_CHILD | WS_VISIBLE | BS_BITMAP, 450 + i * 40 + 8, 25, 40, 40, hWnd, (HMENU)(i + 8), NULL, NULL);
			HANDLE  hbitmap = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(listHinh[i + 3]), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

			SendMessage(hwndCongCu[i], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbitmap);
		}

		//hDlgModeless = CreateDialog(hInstance, szTemplate, hwndParent, DialogProc);
		break;

	}

	case WM_MOUSEMOVE:
	{
		xRight = LOWORD(lParam);
		yBottom = HIWORD(lParam);

		if (flag == true) {
			hdc = GetDC(hWnd);
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			for (int i = 1; i <= lgpen.lopnWidth.x; i++) {
				for (int j = 1; j <= lgpen.lopnWidth.y; j++) {
					SetPixel(hdc, x + i, y + j, lgpen.lopnColor);
				}
			}
			ReleaseDC(hWnd, hdc);
		}

		if (flag2 == true) {
			hdc = GetDC(hWnd);
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			for (int i = 1; i < 10; i++) {
				for (int j = 1; j < 10; j++) {
					SetPixel(hdc, x + i, y + j, RGB(255, 255, 255));
				}
			}
			ReleaseDC(hWnd, hdc);
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		xLeft = LOWORD(lParam);
		yTop = HIWORD(lParam);
		if (Hinh == 9)
		{
			DestroyWindow(GetDlgItem(hWnd, ID_Edit));
			txtXX = LOWORD(lParam);
			txtYY = HIWORD(lParam);
		}
		else if (Hinh == 8)
		{
			if (flag == true) flag = false;
			else flag = true;
		}
		else if (Hinh == 10) {
			if (flag2 == true) flag2 = false;
			else flag2 = true;
		}
		else if (Hinh == 11) {
			hdc = GetDC(hWnd);
			COLORREF color = GetPixel(hdc, xLeft, yTop),color1;
			queue<pair<int, int>> poin;
			poin.push({ xLeft,yTop });
			map<pair<int, int>, int> kt;
			while(!poin.empty()) {
				color1 = GetPixel(hdc, poin.front().first, poin.front().second);
				if (kt[poin.front()] != 1 && color ==color1) {
					kt[poin.front()] = 1;
					SetPixel(hdc, poin.front().first, poin.front().second, colorToMau);
					poin.push({ poin.front().first + 1, poin.front().second });
					poin.push({ poin.front().first , poin.front().second - 1});
					poin.push({ poin.front().first - 1, poin.front().second });
					poin.push({ poin.front().first , poin.front().second + 1});
				}
				poin.pop();
			}
			ReleaseDC(hWnd, hdc);
		}
		break;
	}
	case WM_LBUTTONUP:
	{
		xRight = LOWORD(lParam);
		yBottom = HIWORD(lParam);
		hdc = GetDC(hWnd);
		hPenHinh = CreatePenIndirect(&lgpen);
		SelectObject(hdc, hPenHinh);
		if (Hinh == 0)
		{
			SelectObject(hdc, hbrushHinh);
			Rectangle(hdc, xLeft, yTop, xRight, yBottom);
		}
		else if (Hinh == 1)
		{
			SelectObject(hdc, hbrushHinh);
			Ellipse(hdc, xLeft, yTop, xRight, yBottom);
		}
		else if (Hinh == 2)
		{
			SelectObject(hdc, hbrushHinh);
			POINT apt[] = { {xLeft, yBottom}, {xLeft, yTop}, {xRight, yBottom}, {xLeft, yBottom}, {xRight, yTop }, {xRight,yBottom} };
			Polygon(hdc, apt, 6);
		}
		else if (Hinh == 9) {
			hwndEdit = CreateWindow(TEXT("edit"), TEXT(""),
				WS_CHILD | WS_VISIBLE | WS_BORDER |
				ES_MULTILINE |
				ES_AUTOVSCROLL |
				WS_VSCROLL,
				xLeft, yTop, xRight - xLeft, yBottom - yTop, hWnd, (HMENU)ID_Edit, NULL, NULL);
			//SetFocus(hwndEdit);
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
		case ID_Edit:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				TCHAR text[_MAX_PATH];
				for (int i = 0; i < _MAX_PATH; i++) {
					text[i] = 2;
				}
				SendMessage(GetDlgItem(hWnd, ID_Edit), WM_GETTEXT, sizeof(text) / sizeof(text[0]), (LPARAM)text);
				hdc = GetDC(hWnd);
				SelectObject(hdc, hFont);
				SetTextColor(hdc, colorToMau);
				int dem = 0;
				for (int i = 0; i < _MAX_PATH; i++)
				{
					if (text[i] != 2) {
						dem++;
					}
					else break;
				}
				TextOut(hdc, xLeft, yTop, (LPCWSTR)text, dem);  //hiểm thị tên
				ReleaseDC(hWnd, hdc);
				break;
			}
			break;
		case 0:
			Hinh = 0;
			break;
		case 1:
			Hinh = 1;
			break;
		case 2:
			Hinh = 2;
			break;
		case 3:
			if (mauVien == true) {
				lgpen.lopnColor = color[0];
				mauVien = false;
			}
			else {
				colorToMau = color[0];
				hbrushHinh = hBrush[0];
			}
			break;
		case 4:
			if (mauVien == true) {
				lgpen.lopnColor = color[1];
				mauVien = false;
			}
			else {
				colorToMau = color[1];
				hbrushHinh = hBrush[1];
			}
			break;
		case 5:
			if (mauVien == true) {
				lgpen.lopnColor = color[2];
				mauVien = false;
			}
			else {
				colorToMau = color[2];
				hbrushHinh = hBrush[2];
			}
			break;
		case 6:
			if (mauVien == true) {
				lgpen.lopnColor = color[3];
				mauVien = false;
			}
			else {
				colorToMau = color[3];
				hbrushHinh = hBrush[3];
			}
			break;
		case 7:
			if (mauVien == true) {
				lgpen.lopnColor = color[4];
				mauVien = false;
			}
			else {
				colorToMau = color[4];
				hbrushHinh = hBrush[4];
			}
			break;
		case 8:
			Hinh = 8;
			break;
		case 9:
			Hinh = 9;
			break;
		case 10:
			Hinh = 10;
			break;
		case 11:
			Hinh = 11;
			break;
		case 12:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DialogPaint);
			Hinh = 12;
			break;
		case 13:
			Hinh = 13;
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DialogFont);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(About), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
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
INT_PTR CALLBACK DialogFont(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int nIndexFondText = -1, nIndexTextSize = -1;
	bool kt = false;
	HWND cboTextFond = NULL, cboTextSize;
	LPCTSTR textSize[] = { TEXT("5"), TEXT("10"),TEXT("15"),TEXT("20") };
	LPCTSTR textFont[] = { TEXT(".VnGothicH"), TEXT(".VnBook-AntiquaH"),TEXT(".VnShelley Allegro"),TEXT("Broadway") };
	DWORD kieuChuB = FALSE, kieuChuU = FALSE, kieuChuI = FALSE;
	switch (message)
	{
	case WM_INITDIALOG:
		SetWindowPos(GetDlgItem(hDlg, IDC_FontText), 0, 0, 0, 100, 200, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		cboTextFond = GetDlgItem(hDlg, IDC_FontText);
		for (int Count = 0; Count < 4; Count++)
		{
			SendMessage(cboTextFond, CB_ADDSTRING, Count, reinterpret_cast<LPARAM>((LPCTSTR)textFont[Count]));
		}

		SetWindowPos(GetDlgItem(hDlg, IDC_TextSize), 0, 0, 0, 100, 100, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		cboTextSize = GetDlgItem(hDlg, IDC_TextSize);
		for (int Count = 0; Count < 4; Count++)
		{
			SendMessage(cboTextSize, CB_ADDSTRING, Count, reinterpret_cast<LPARAM>((LPCTSTR)textSize[Count]));
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OK:
			if (SendMessage(GetDlgItem(hDlg, IDC_B), BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				kieuChuB = TRUE;
				kt = true;
			}
			else if (SendMessage(GetDlgItem(hDlg, IDC_I), BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				kieuChuI = TRUE;
				kt = true;
			}
			else if (SendMessage(GetDlgItem(hDlg, IDC_U), BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				kieuChuU = TRUE;
				kt = true;
			}
			nIndexFondText = SendMessage(GetDlgItem(hDlg, IDC_FontText), CB_GETCURSEL, 0, 0);
			nIndexTextSize = SendMessage(GetDlgItem(hDlg, IDC_TextSize), CB_GETCURSEL, 0, 0);
			if (nIndexFondText == -1 || nIndexTextSize == -1 || kt == false)
			{
				MessageBox(NULL, TEXT("Vui Long Nhap Day Du"), TEXT("Message"), MB_OK);
			}
			else
			{
				TCHAR txtTextSize[_MAX_PATH];
				hdc = BeginPaint(hDlg, &ps);

				SendMessage(GetDlgItem(hDlg, IDC_TextSize), CB_GETLBTEXT, nIndexTextSize, (LPARAM)txtTextSize);
				int weight = _ttoi(txtTextSize); // convert to int
				hFont = CreateFont(weight * 2, weight, 0, 0, FW_DONTCARE, kieuChuI, kieuChuU, kieuChuB, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
					CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, textFont[nIndexFondText]);
				EndDialog(hDlg, LOWORD(wParam));
			}
			break;
		default:
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, TRUE);
		break;
	default:
		return (INT_PTR)FALSE;

	}
	return (INT_PTR)TRUE;
}

INT_PTR CALLBACK DialogPaint(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int nIndex;
	HWND cboTextSize;
	LPCTSTR textSize[] = { TEXT("1"), TEXT("5"), TEXT("10"),TEXT("15"),TEXT("20") };
	switch (message)
	{
	case WM_INITDIALOG:
		SetWindowPos(GetDlgItem(hDlg, IDC_TextSize), 0, 0, 0, 100, 100, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		cboTextSize = GetDlgItem(hDlg, IDC_TextSize);
		for (int Count = 0; Count < 5; Count++)
		{
			SendMessage(cboTextSize, CB_ADDSTRING, Count, reinterpret_cast<LPARAM>((LPCTSTR)textSize[Count]));
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OK11:
			bool kt = false;
			if (SendMessage(GetDlgItem(hDlg, IDC_NetLien), BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				lgpen.lopnStyle = PS_SOLID;
				kt = true;
			}
			else if (SendMessage(GetDlgItem(hDlg, IDC_NetDut), BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				lgpen.lopnStyle = PS_DASH;
				kt = true;
			}
			else if (SendMessage(GetDlgItem(hDlg, IDC_NetCham), BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				lgpen.lopnStyle = PS_DOT;
				kt = true;
			}
			else if (SendMessage(GetDlgItem(hDlg, IDC_NetChamVaNgachNgang), BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				lgpen.lopnStyle = PS_DASHDOT;
				kt = true;
			}
			else if (SendMessage(GetDlgItem(hDlg, IDC_NetNgacNgangVaDauKep), BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				lgpen.lopnStyle = PS_DASHDOTDOT;
				kt = true;
			}
			TCHAR pFilename[_MAX_PATH];
			nIndex = -1;
			nIndex = SendMessage(GetDlgItem(hDlg, IDC_TextSize), CB_GETCURSEL, 0, 0);
			if (kt == false || nIndex == -1)
			{
				MessageBox(NULL, TEXT("Vui Long Nhap Day Du"), TEXT("Message"), MB_OK);
			}
			else {
				SendMessage(GetDlgItem(hDlg, IDC_TextSize), CB_GETLBTEXT, nIndex, (LPARAM)pFilename);
				int weight = _ttoi(pFilename); // convert to int
				lgpen.lopnWidth = { weight , weight };
				lgpen.lopnColor = RGB(0, 0, 0);
				EndDialog(hDlg, LOWORD(wParam));
				mauVien = true;
			}
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, TRUE);
		break;
	default:
		return (INT_PTR)FALSE;

	}
	return (INT_PTR)TRUE;
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