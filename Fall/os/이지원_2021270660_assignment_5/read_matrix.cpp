#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>

#define MAX_LINE 1000
#define DELIM " \r\t"  // 공백, 탭 등을 구분자로 설정

void read_matrix(std::vector<std::vector<int>>& A, int& Ar, int& Ac, const char* filename) {
    char line[MAX_LINE], * tok;
    FILE* fp;

    // 파일 열기
    if (!(fp = fopen(filename, "r"))) {
        printf("ERROR: file open\n");
        exit(0);
    }

    Ar = 0;
    A.clear();  // 기존의 데이터 비우기

    // 파일에서 한 줄씩 읽어오기
    while (fgets(line, MAX_LINE, fp)) {
        tok = strtok(line, DELIM);  // 첫 번째 토큰을 구분자 기준으로 잘라서 가져오기
        std::vector<int> row;  // 각 행을 저장할 벡터
        Ac = 0;

        // 행에 있는 모든 숫자를 읽어서 벡터에 저장
        do {
            row.push_back(atoi(tok));  // 문자열을 정수로 변환하여 벡터에 추가
            Ac++;
        } while (tok = strtok(NULL, DELIM));  // 다음 토큰으로 이동

        A.push_back(row);  // 현재 행을 2D 벡터에 추가
        Ar++;  // 행 개수 증가
    }

    fclose(fp);  // 파일 닫기
}
