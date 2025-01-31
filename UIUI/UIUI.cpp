// UIUI.cpp : Defines the entry point for the application.

#include "stdafx.h"
#include "UIUI.h"
#include <aygshell.h>
#include <fstream>
#include <string>
#include <vector>

#define MAX_LOADSTRING 100

// Added definitions to replace previous #defines
#define UP 14952
#define DOWN 14951
#define CENTER 10

bool GifEnabled = false;
int MaxFrames = 0;
int frame = 0;

// Global Variables:
HINSTANCE g_hInst;            
HWND g_hWndMenuBar;          
TCHAR g_szTitle[MAX_LOADSTRING];        
TCHAR g_szWindowClass[MAX_LOADSTRING];  

// Forward declarations
ATOM            MyRegisterClass(HINSTANCE hInstance);
BOOL            InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

// Configuration structure to centralize settings
struct AppConfig {
    LPCTSTR NavPath;
    LPCTSTR NName;
    LPCTSTR NIcon;
    LPCTSTR CName;
    LPCTSTR CIcon;
    LPCTSTR BG;
    int nx, ny, cx, cy, mx, my;
    int ns, cs, ms;
    int STYLEZ;
    int mode;
	int sx, sy;
} g_Config;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    MSG msg;
    if (!InitInstance(hInstance, nCmdShow)) return FALSE;

    while (GetMessage(&msg, NULL, 0, 0)) 
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASS wc = {0};
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_UIUI));
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = g_szWindowClass;

    return RegisterClass(&wc);
}

bool LoadConfiguration()
{
    std::ifstream in("SDMMC//config//Configuration.cfg");
    
    if (!in.is_open())
    {
        // Default configuration if file not found
        g_Config.NavPath = L"SDMMC//IGO.exe";
        g_Config.NName = L"GPS";
        g_Config.NIcon = L"Windows//winmedia.bmp";
        g_Config.CName = L"Configuration";
        g_Config.CIcon = L"Windows//bt_help.bmp";
        g_Config.BG = L"Windows//alerts.bmp";
        g_Config.nx = 100;
        g_Config.ny = CENTER;
        g_Config.cx = 200;
        g_Config.cy = CENTER;
        g_Config.mx = 500;
        g_Config.my = CENTER;
        g_Config.ns = 30;
        g_Config.cs = 30;
        g_Config.ms = 30;
        g_Config.STYLEZ = UP;
        g_Config.mode = 0;

        MessageBox(NULL, L"Configuration file not found, will use default", L"Warning", MB_ICONWARNING);
        return false;
    }

	std::string param;
	std::string val;
    while (in >> param >> val)
    {
        // Simplified configuration parsing
		if (param == "NX") if (val == "CENTER") { g_Config.nx = CENTER; } else { g_Config.nx = atoi(val.c_str()); }
		else if (param == "NY") if (val == "CENTER") { g_Config.ny = CENTER; } else { g_Config.ny = atoi(val.c_str()); }
		else if (param == "NSize") g_Config.ns = atoi(val.c_str());

		else if (param == "CX") if (val == "CENTER") { g_Config.cx = CENTER; } else { g_Config.cx = atoi(val.c_str()); }
		else if (param == "CY") { if (val == "CENTER") { g_Config.cy = CENTER; } else { g_Config.cy = atoi(val.c_str()); } }


		else if (param == "ConfSize") g_Config.cs = atoi(val.c_str());
		else if (param == "GIFEnabled" && val == "TRUE") {
			GifEnabled = true;
		} else if (param == "GIFEnabled" && val == "FALSE") {
			GifEnabled = false;
		}
		else if (param == "MaxFrames" && GifEnabled == true) { MaxFrames = atoi(val.c_str()); } 

		

		else if (param == "BG") {
			wchar_t bgPath[256];
			MultiByteToWideChar(CP_ACP, 0, val.c_str(), -1, bgPath, 256);
			if (g_Config.BG != NULL) { free((void*)g_Config.BG); }
			size_t len = wcslen(bgPath) + 1; wchar_t* newPath = (wchar_t*)malloc(len * sizeof(wchar_t));
			wcscpy(newPath, bgPath);
			g_Config.BG = newPath;
		}
		else if (param == "NIcon") {
			wchar_t iconPath[256];
			MultiByteToWideChar(CP_ACP, 0, val.c_str(), -1, iconPath, 256);
			if (g_Config.NIcon != NULL) { free((void*)g_Config.NIcon); }
			size_t len = wcslen(iconPath) + 1; wchar_t* newPath = (wchar_t*)malloc(len * sizeof(wchar_t));
			wcscpy(newPath, iconPath);
			g_Config.NIcon = newPath;
		}
		else if (param == "ConfIco") {
			wchar_t iconPath[256];
			MultiByteToWideChar(CP_ACP, 0, val.c_str(), -1, iconPath, 256);
			if (g_Config.CIcon != NULL) { free((void*)g_Config.CIcon); }
			size_t len = wcslen(iconPath) + 1; wchar_t* newPath = (wchar_t*)malloc(len * sizeof(wchar_t));
			wcscpy(newPath, iconPath);
			g_Config.CIcon = newPath;
		}

    }

    return true;
}

bool buttonpressed(int x, int y, int size, LPARAM lParam) 
{
    int mouseX = LOWORD(lParam);
    int mouseY = HIWORD(lParam);

    return (mouseX >= x && mouseX <= x + size && 
            mouseY >= y && mouseY <= y + size);
}

int centermode(int argv, int size, int scr)
{
	return argv = (scr - size) / 2;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    g_hInst = hInstance;
    SHInitExtraControls();

    HWND hWnd = FindWindow(g_szWindowClass, g_szTitle);    
    if (hWnd) 
    {
        SetForegroundWindow((HWND)((ULONG)hWnd | 0x00000001));
        return FALSE;
    } 

    if (!MyRegisterClass(hInstance)) return FALSE;

    // Create main window
    hWnd = CreateWindow(g_szWindowClass, 
                       L"SHELLEX_SYG", 
                       WS_VISIBLE | WS_POPUP,
                       CW_USEDEFAULT, CW_USEDEFAULT,
                       GetSystemMetrics(SM_CXSCREEN),
                       GetSystemMetrics(SM_CYSCREEN),
                       NULL, NULL, hInstance, NULL);

    if (!hWnd) return FALSE;

    // Remove window decorations
    LONG style = GetWindowLong(hWnd, GWL_STYLE);
    style &= ~(WS_CAPTION | WS_THICKFRAME | WS_SYSMENU);
    SetWindowLong(hWnd, GWL_STYLE, style);

    SHFullScreen(hWnd, SHFS_HIDETASKBAR | SHFS_HIDESTARTICON | SHFS_HIDESIPBUTTON);
    
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

HBITMAP icons, Background;

bool drawicons(HDC hdc, int x, int y, int size, LPCTSTR Filename) 
{
    HBITMAP hBitmap = SHLoadDIBitmap(Filename);  // Correct type assignment
    if (!hBitmap) 
    {
        DWORD errorCode = GetLastError(); 
        wchar_t errorMsg[256];
        swprintf(errorMsg, L"Error: Failed to load bitmap. (Error Code: %lu)", errorCode);
        MessageBoxW(NULL, errorMsg, L"Error", MB_ICONERROR | MB_OK);
        return false;
    }

    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);

    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, hBitmap);

    // Stretch and draw the bitmap
    StretchBlt(hdc, x, y, size, size, hdcMem, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

    // Restore old bitmap and clean up
    SelectObject(hdcMem, hOldBmp);
    DeleteDC(hdcMem);
    DeleteObject(hBitmap);  // Free the loaded bitmap

    return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static SHACTIVATEINFO s_sai;
    static int sx, sy; // Screen sizes
    

    switch (message) 
    {
        case WM_CREATE:
        {
            memset(&s_sai, 0, sizeof(s_sai));
            s_sai.cbSize = sizeof(s_sai);

            SHMENUBARINFO mbi = {0};
            mbi.hwndParent = hWnd;
            mbi.hInstRes   = g_hInst;
            mbi.dwFlags    = SHCMBF_HIDDEN;
            g_hWndMenuBar = SHCreateMenuBar(&mbi) ? mbi.hwndMB : NULL;

            LoadConfiguration();

            HDC hdc = GetDC(NULL);
            if (hdc) {
                sx = GetDeviceCaps(hdc, HORZRES);
                sy = GetDeviceCaps(hdc, VERTRES);
                ReleaseDC(NULL, hdc);
            }



            // Center positioning
			if (g_Config.nx == CENTER) { g_Config.nx = centermode(g_Config.nx, g_Config.ns, sx); }
			if (g_Config.ny == CENTER) { g_Config.ny = centermode(g_Config.ny, g_Config.ns, sy); }
			if (g_Config.cx == CENTER) { g_Config.cx = centermode(g_Config.cx, g_Config.cs, sx); }
			if (g_Config.cy == CENTER) { g_Config.cy = centermode(g_Config.cy, g_Config.cs, sy); }
			
			MessageBox(hWnd, g_Config.CIcon, L"Detection", MB_OK);
            // Load bitmaps

            Background = SHLoadDIBitmap(g_Config.BG);

			SetTimer(hWnd, 1, 100, NULL);

        }
        break;

		case WM_TIMER:	
		{	
			if (GifEnabled == true) {

				TCHAR filefromframe[256];
				if (Background) {
					DeleteObject(Background);
				}

				// Debugging
				swprintf(filefromframe, L"SDMMC\\BG\\frame%d.bmp", frame);

				// Update the bitmap
				Background = SHLoadDIBitmap(filefromframe);

				if (frame > MaxFrames) {
					frame = 1;
				} else {
					frame++;
				}

				// Trigger repaint
				InvalidateRect(hWnd, NULL, FALSE);
			}
		}

        case WM_LBUTTONDOWN:
        {
            if (g_Config.mode == 0 && buttonpressed(g_Config.nx, g_Config.ny, g_Config.ns, lParam)) 
            {
                MessageBox(hWnd, L"A Square was drawn!", L"Detection", MB_OK);
            }
        }
        break;

        case WM_PAINT:
        {

            icons = SHLoadDIBitmap(g_Config.NIcon);


			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			HDC hdcMem = CreateCompatibleDC(hdc);

				HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, Background);
				BITMAP bmp;
				GetObject(Background, sizeof(BITMAP), &bmp);

				StretchBlt(hdc, 0, 0, sx, sy, hdcMem, 0, 0, 
						   bmp.bmWidth, bmp.bmHeight, SRCCOPY);

				SelectObject(hdcMem, hOldBitmap);

			// Draw navigation icon
			drawicons(hdc,g_Config.nx,g_Config.ny,g_Config.ns,g_Config.NIcon);
			// Draw settings icon

			drawicons(hdc,g_Config.cx,g_Config.cy,g_Config.cs,g_Config.CIcon);
			// Clean up
			DeleteDC(hdcMem);

			EndPaint(hWnd, &ps);

			DeleteObject(Background);
			DeleteObject(icons);
        }
        break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}