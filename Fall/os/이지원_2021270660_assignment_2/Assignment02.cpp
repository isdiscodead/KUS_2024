#pragma once

#include <iostream>
using namespace std;

// 출력 포맷 지정을 위한 라이브러리
#include <iomanip>

// EnumProcesses()를 사용하기 위한 헤더 파일, 라이브러리 링크 
#define PSAPI_VERSION 1 // OS 문제 해결을 위한 옵션 define ... 
#include <windows.h>
#include <psapi.h>
#pragma comment( lib, "Psapi.lib" ) // #pragma comment(lib, "라이브러리.lib") -> 명시적 라이브러리 링크

// 현재 시간을 출력하는 함수 
void printTime() {
   SYSTEMTIME lt;
   // GetSystem() -> UTC 기준... 따라서 GetLocalTime()을 사용해 시간을 가져옴
   GetLocalTime(&lt); // get local time 
   cout << lt.wYear << "-";
   cout.width(2);
   cout.fill('0');
   cout << lt.wMonth << "-";
   cout.width(2);
   cout << lt.wDay << "  ";
   cout.width(2);
   cout << lt.wHour << ":";
   cout.width(2);
   cout << lt.wMinute << ":";
   cout.width(2);
   cout << lt.wSecond << " -> ";
}

// 현재 프로세스 수를 반환하는 함수
int getProcessNum() {
   DWORD aProcesses[1024];
   DWORD cbNeeded;

   // procesees가 없을 경우 return 1 ( 오류 )
   if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
      return 1;
   }
      
   return cbNeeded / sizeof(DWORD); // 프로세스의 개수 반환 
}

int main(void) {

   while (true) {
      printTime();
      cout << getProcessNum() << endl;
      Sleep(1000); // 1초 정지 
   }
   return 0;
}