#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>

#define MAX_LINE 1000
#define DELIM " \r\t"  // ����, �� ���� �����ڷ� ����

void read_matrix(std::vector<std::vector<int>>& A, int& Ar, int& Ac, const char* filename) {
    char line[MAX_LINE], * tok;
    FILE* fp;

    // ���� ����
    if (!(fp = fopen(filename, "r"))) {
        printf("ERROR: file open\n");
        exit(0);
    }

    Ar = 0;
    A.clear();  // ������ ������ ����

    // ���Ͽ��� �� �پ� �о����
    while (fgets(line, MAX_LINE, fp)) {
        tok = strtok(line, DELIM);  // ù ��° ��ū�� ������ �������� �߶� ��������
        std::vector<int> row;  // �� ���� ������ ����
        Ac = 0;

        // �࿡ �ִ� ��� ���ڸ� �о ���Ϳ� ����
        do {
            row.push_back(atoi(tok));  // ���ڿ��� ������ ��ȯ�Ͽ� ���Ϳ� �߰�
            Ac++;
        } while (tok = strtok(NULL, DELIM));  // ���� ��ū���� �̵�

        A.push_back(row);  // ���� ���� 2D ���Ϳ� �߰�
        Ar++;  // �� ���� ����
    }

    fclose(fp);  // ���� �ݱ�
}
