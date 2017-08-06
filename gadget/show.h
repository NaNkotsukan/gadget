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
    //HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 200, 150));
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));





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
    hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    SelectObject(hdc, hPen);

    for (int n = core; n < core + 1; n++) {
        //Sleep(1000);
        PdhCollectQueryData(hQuery[n]);
        PdhGetFormattedCounterValue(hCounter[n], PDH_FMT_LONG, NULL, &fntValue);
        cpuRatio.push_front(fntValue.longValue);//CPUŽg—p—¦
 
        time_t t = time(NULL);

        PAINTSTRUCT ps;

        //HPEN        hPen;
        //hPen = CreatePen(PS_SOLID, 10, RGB(0, 200, 150));
        //hOldPen = (HPEN)SelectObject(hdc, hPen);
        MoveToEx(hdc, 50, 50, NULL);
        LineTo(hdc, 50, 200);
        LineTo(hdc, 350, 200); 
        LineTo(hdc, 350, 50);
        LineTo(hdc, 50, 50);
        if (cpuRatio.size() > 100)cpuRatio.pop_back()   ;
        //hPen = CreatePen(PS_SOLID, 20, RGB(0, 200, 150));
        //hOldPen = (HPEN)SelectObject(hdc, hPen);
        MoveToEx(hdc, 50+100-cpuRatio.back(),300, NULL);
        EndPaint(hWnd, &ps);


        //for (int i = cpuRatio.size() - 1;i>0;--i) {
        //    LineTo(hdc, 50 + 100 - cpuRatio[i]+10, 350-i);
        //}
        //InvalidateRect(hWnd, NULL, TRUE);
    }
}


void show::showTime(HWND hWnd, HDC hdc) {

    PAINTSTRUCT ps;
    TCHAR   szstr[256];
    time_t t = time(NULL);
    _stprintf_s(szstr, _T("%s"), ctime(&t));
    TextOut(hdc, 5, 5, szstr, _tcslen(szstr) - 1);
}

void show::test(HWND hWnd, HDC hdc) {

}