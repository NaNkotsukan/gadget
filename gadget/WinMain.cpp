#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include"show.h"
#include<time.h>

auto a=show();
HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
    
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    static TCHAR szWindowClass[] = _T("gadget");

    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,_T("error"),_T("gadget"),NULL);

        return 1;
    }

    hInst = hInstance;
    HWND hWnd = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_LAYERED | /*WS_EX_TRANSPARENT |*/ WS_EX_TOPMOST, szWindowClass, _T(""), WS_POPUP | WS_SYSMENU | WS_VISIBLE, 1500, 0, 420, 1080, NULL, NULL, hInstance, NULL);
    if (!hWnd)
    {
        MessageBox(NULL, _T("ウィンドウ生成に失敗しました!"), _T("Gadget"), NULL);
        return 1;
    }

    SetLayeredWindowAttributes(hWnd, RGB(255, 255, 255), 10, LWA_COLORKEY);
    //SetLayeredWindowAttributes(hWnd, RGB(255, 0, 0), 10, LWA_COLORKEY);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    TCHAR   szstr[256];


    switch (message) {
    case WM_CREATE:
        SetTimer(hWnd, 1, 1000, NULL);
        break;
    case WM_TIMER:
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:

        hdc = BeginPaint(hWnd, &ps);
        //a.test(hWnd,hdc);
        a.showTime(hWnd,hdc);
        a.showCpu(hWnd,hdc);
        EndPaint(hWnd, &ps);

        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}