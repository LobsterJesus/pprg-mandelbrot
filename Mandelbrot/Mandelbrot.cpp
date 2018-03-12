// Mandelbrot.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <omp.h>
#include "Mandelbrot.h"

#define MAX_LOADSTRING 100
#define MAX_ITERATIONS 1000
#define MOVE_OFFSET 0.1

// My global variables
double _mandelZoom = 1;
double _mandelOffsetX = -0.5;
double _mandelOffsetY = 0;

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
    LoadStringW(hInstance, IDC_MANDELBROT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MANDELBROT));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MANDELBROT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MANDELBROT);
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

   // width = CW_USEDEFAULT

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 1024, 1024, nullptr, nullptr, hInstance, nullptr);

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
//  PURPOSE:  Processes messages for the main window.
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
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
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
			PaintWindow(hWnd);
        }
        break;
	case WM_KEYDOWN:

		switch (wParam)
		{
		case VK_OEM_PLUS:
		case VK_ADD:

			_mandelZoom += 1;
			PaintWindow(hWnd);

			break;

		case VK_OEM_MINUS:
		case VK_SUBTRACT:
			
			_mandelZoom -= 1;
			PaintWindow(hWnd);
			break;

		case VK_RIGHT:

			_mandelOffsetX += MOVE_OFFSET;
			PaintWindow(hWnd);
			break;

		case VK_LEFT:

			_mandelOffsetX -= MOVE_OFFSET;
			PaintWindow(hWnd);
			break;

		case VK_UP:

			_mandelOffsetY -= MOVE_OFFSET;
			PaintWindow(hWnd);
			break;

		case VK_DOWN:

			_mandelOffsetY += MOVE_OFFSET;
			PaintWindow(hWnd);
			break;

		default:
			break;
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

void Trace(char* message)
{
	OutputDebugStringA(message);
}

void Trace(char* message, double var)
{
	char buffer[200];
	sprintf(buffer, message, var);
	OutputDebugStringA(buffer);
}

void PaintWindow(const HWND hWnd)
{
	clock_t begin, end;
	double timeSpent;
	RECT rect;
	int width;
	int height;

	PAINTSTRUCT ps;
	HDC hDc = BeginPaint(hWnd, &ps);

	if (GetClientRect(hWnd, &rect))
	{
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
	}

	// Invalidate rect for redrawing actions (zoom, move)
	InvalidateRect(hWnd, &rect, FALSE);

	// Create temporary bitmap to draw onto
	COLORREF *arr = (COLORREF*)calloc(1024 * 1024, sizeof(COLORREF));
	HBITMAP map = CreateBitmap(1024, 1024, 1 /* color panes? */, 8 * 4, (void*)arr);

	// Create temporary device context
	HDC hMem = CreateCompatibleDC(hDc);

	// Select bitmap in temp device context
	SelectObject(hMem, map);

	begin = clock();
	
	//PaintMandelbrot(hMem, width, height, _mandelZoom, _mandelOffsetX, _mandelOffsetY);
	PaintMandelbrotParallel(hMem, width, height, _mandelZoom, _mandelOffsetX, _mandelOffsetY);


	end = clock();
	timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

	// Copy bitmap to current window client area
	BitBlt(hDc, 0, 0, width, height, hMem, 0, 0, SRCCOPY);

	Trace("Elapsed time: %.2lf seconds.\n", timeSpent);

	DeleteDC(hMem);
	EndPaint(hWnd, &ps);
}

void PaintMandelbrotParallel(
	const HDC hDc,
	const int width,
	const int height,
	const double zoom,
	const double offsetX,
	const double offsetY)
{

	omp_set_num_threads(4);

#pragma omp parallel for schedule(guided)
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
			int* color = CalculatePixel(x, y, width, height, zoom, offsetX, offsetY);

#pragma omp critical
			SetPixel(hDc, x, y, RGB(color[0], color[1], color[2]));
		}
}

void PaintMandelbrot(
	const HDC hDc, 
	const int width, 
	const int height,
	const double zoom,
	const double offsetX,
	const double offsetY)
{
	for (int y = 0; y < height; y++)
	for (int x = 0; x < width; x++)
	{
		int* color = CalculatePixel(x, y, width, height, zoom, offsetX, offsetY);
		SetPixel(hDc, x, y, RGB(color[0], color[1], color[2]));
	}
}

int* CalculatePixel(
	const int imgX, 
	const int imgY,
	const int imageWidth,
	const int imageHeight, 
	const double zoom,
	const double offsetX,
	const double offsetY)
{
	// Normalize real and imaginary parts of the complex number
	double cx = 1.5 * (imgX - imageWidth / 2) / (0.5 * zoom * imageWidth) + offsetX;
	double cy = (imgY - imageHeight / 2) / (0.5 * zoom * imageHeight) + offsetY;
	double zx = cx;
	double zy = cy;

	for (int i = 0; i < MAX_ITERATIONS; i++)
	{
		double x = (zx * zx - zy * zy) + cx;
		double y = (zy * zx + zx * zy) + cy;

		// diverge , produce nice color
		if ((x*x + y*y) > 4)
		{
			double z = sqrt(x*x + y*y);
			int brightness = 256.0 * log2(1.75 + i - log2(log2(z))) / log2(double(MAX_ITERATIONS));
			
			//paint pixel px, py in a color depending on n
			return new int[3] { 150, brightness, brightness };
		}

		zx = x;
		zy = y;
	}
	
	return new int[3] {0, 0, 0};
}