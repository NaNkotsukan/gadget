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
    void showCpu(HWND hWnd);

private:
    int ram;
    int core;
    PDH_HQUERY hQuery[17];
    PDH_HCOUNTER hCounter[17];
    PDH_FMT_COUNTERVALUE fntValue;
    std::deque<char> cpuRatio;



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

void show::showCpu(HWND hWnd) {
    for (int n = core; n < core + 1; n++) {
        //Sleep(1000);
        PdhCollectQueryData(hQuery[n]);
        PdhGetFormattedCounterValue(hCounter[n], PDH_FMT_LONG, NULL, &fntValue);
        cpuRatio.push_front(fntValue.longValue);//CPU使用率
 
        time_t t = time(NULL);

        HDC hdc;
        PAINTSTRUCT ps;

        hdc = BeginPaint(hWnd, &ps);
        TCHAR   szstr[256];
        _stprintf_s(szstr, _T("CPU使用率は%dです"), cpuRatio[0]);

        //_stprintf_s(szstr, _T("%s"),  ctime(&t));
        TextOut(hdc, 50, 50,szstr, _tcslen(szstr));
        EndPaint(hWnd, &ps);



        hdc = BeginPaint(hWnd, &ps);
        HPEN        hPen, hOldPen;
        hPen = CreatePen(PS_SOLID, 1, RGB(0, 200, 150));
        hOldPen = (HPEN)SelectObject(hdc, hPen);
        MoveToEx(hdc, 50, 50, NULL);
        LineTo(hdc, 50, 200);
        LineTo(hdc, 350, 200); 
        LineTo(hdc, 350, 50);
        LineTo(hdc, 50, 50);
        if (cpuRatio.size() > 100)cpuRatio.pop_back();
        hPen = CreatePen(PS_SOLID, 20, RGB(0, 200, 150));
        hOldPen = (HPEN)SelectObject(hdc, hPen);
        MoveToEx(hdc, 50+100-cpuRatio.back(),300, NULL);
        for (int i = cpuRatio.size() - 1;i>0;--i) {
            LineTo(hdc, 50 + 100 - cpuRatio[i]+10, 350-i);
        }
    }
}