#pragma once

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <conio.h>
#include <pdh.h>
#include <deque>
#include<time.h>
#pragma comment(lib, "pdh.lib")

    
class show
{
public:
    show();
    ~show();
    void showCpu(HWND, HDC);
    void showTime(HWND,HDC );
    void test(HWND, HDC);
private:
    int ram;
    int core;
    PDH_HQUERY hQuery[17];
    PDH_HCOUNTER hCounter[17];
    PDH_FMT_COUNTERVALUE fntValue;
    std::deque<char> cpuRatio;
    HPEN hPenG2 = CreatePen(PS_SOLID, 1, RGB(0, 220, 150));
    HPEN hPenG = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));





};

show::show()
{
    MEMORYSTATUSEX msex = { sizeof(MEMORYSTATUSEX) };
    GlobalMemoryStatusEx(&msex);

    int n;
    char query[128];
 

    core = atoi(getenv("NUMBER_OF_PROCESSORS"));
    for (n = 0; n < core + 1; n++) {
        PdhOpenQuery(NULL, 0, &hQuery[n]);
    }

    for (n = 0; n < core; n++) {
        PdhAddCounter(hQuery[n], query, 0, &hCounter[n]);
        PdhCollectQueryData(hQuery[n]);
    }
    PdhAddCounter(hQuery[core], ("\\Processor(_Total)\\% Processor Time"), 0, &hCounter[core]);
    PdhCollectQueryData(hQuery[core]);
}

show::~show()
{
}

void show::showCpu(HWND hWnd, HDC hdc) {
    //hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    SelectObject(hdc, hPenG);

    for (int n = core; n < core + 1; n++) {
        //Sleep(1000);
        PdhCollectQueryData(hQuery[n]);
        PdhGetFormattedCounterValue(hCounter[n], PDH_FMT_LONG, NULL, &fntValue);
        cpuRatio.push_front(fntValue.longValue);//CPUŽg—p—¦
 
        PAINTSTRUCT ps;

        //HPEN        hPen;
        //hPen = CreatePen(PS_SOLID, 10, RGB(0, 200, 150));
        //hOldPen = (HPEN)SelectObject(hdc, hPen);
        MoveToEx(hdc, 50, 50, NULL);
        LineTo(hdc, 50, 200);
        LineTo(hdc, 350, 200); 
        LineTo(hdc, 350, 50);
        LineTo(hdc, 50, 50);
        if (cpuRatio.size() > 300)cpuRatio.pop_back()   ;
        //hPen = CreatePen(PS_SOLID, 20, RGB(0, 200, 150));
        //hOldPen = (HPEN)SelectObject(hdc, hPen);
        //MoveToEx(hdc,350, 50+100-cpuRatio.back(), NULL);

        //for (int i = cpuRatio.size() - 1;i>0;--i) {
        //    LineTo(hdc,350 - i,50 + 100 - cpuRatio[i]+10);
        //}

        SelectObject(hdc, hPenG2);

        MoveToEx(hdc, 350- cpuRatio.size(), 50 + 100 - cpuRatio.back(), NULL);

        for (int i = cpuRatio.size() - 1;i>0;--i) {
            LineTo(hdc, 350 - i, 50 + 100 - cpuRatio[i]);
        }
        EndPaint(hWnd, &ps);

        //InvalidateRect(hWnd, NULL, TRUE);

        //TCHAR   szstr[256];
        //_stprintf_s(szstr, _T("mazai_%d"), cpuRatio.size());
        //TextOut(hdc, 5, 100, szstr, _tcslen(szstr) - 1);
    }
}


void show::showTime(HWND hWnd, HDC hdc) {
    //LOGFONT lfFont;
    //lfFont.lfHeight = 10;
    //lfFont.lfWeight = FW_EXTRALIGHT;

    PAINTSTRUCT ps;
    TCHAR   szstr[256];
    time_t t = time(NULL);
    _stprintf_s(szstr, _T("%s"), ctime(&t));
    SetTextColor(hdc, RGB(255, 0, 0));
    //hdc = BeginPaint(hWnd, &ps);

    //HFONT  hFont = CreateFontIndirect(&lfFont);

    //SelectObject(hdc, hFont);

    TextOut(hdc, 10, 5, szstr, _tcslen(szstr) - 1);
}

void show::test(HWND hWnd, HDC hdc) {

}