#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include "read_matrix.h"

using namespace std;

// 행렬 곱셈 함수 (멀티스레딩)
vector<vector<int>> matrix_multiply(const vector<vector<int>>& A, const vector<vector<int>>& B, int m, int k, int n) {
    vector<vector<int>> C(m, vector<int>(n, 0));
    vector<thread> threads;

    // 각 C[i][j] 항목을 계산하는 스레드
    auto compute_entry = [&](int i, int j) {
        this_thread::sleep_for(chrono::seconds(1));  // 1초 대기
        for (int l = 0; l < k; ++l) {
            C[i][j] += A[i][l] * B[l][j];
        }
        };

    // 스레드 생성 (각 C[i][j] 항목 계산)
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            threads.push_back(thread(compute_entry, i, j));  // 각 스레드 생성
        }
    }

    // 모든 스레드 종료 대기
    for (auto& th : threads) {
        th.join();  // 스레드 종료 대기
    }

    return C;
}

// 행렬 출력 함수
void print_matrix(const vector<vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << setw(4) << val << " ";
        }
        cout << endl;
    }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        cout << "ERROR: input sequence! Usage: mm <file1> <file2> ...\n";
        return 0;
    }

    vector<vector<int>> A, B, C;
    int m, k, n;

    // 시간 측정 시작
    auto start_time = chrono::high_resolution_clock::now();

    // 첫 번째 행렬 파일 읽기
    read_matrix(A, m, k, argv[1]);

    // 각 파일을 읽어와 행렬 곱셈을 수행
    for (int arg = 1; arg < argc - 1; ++arg) {
        if (arg > 1) {
            A = C;
        }
        
        // 다음 행렬 파일 읽기
        read_matrix(B, k, n, argv[arg + 1]);

        cout << "(" << setw(5) << m << "x" << setw(5) << k << ") X ("
            << setw(5) << k << "x" << setw(5) << n << ") = ("
            << setw(5) << m << "x" << setw(5) << n << ")" << endl;

        // 행렬 곱셈 수행
        C = matrix_multiply(A, B, m, k, n);
    }

    // 시간 측정 끝
    auto end_time = chrono::high_resolution_clock::now();

    // 결과 행렬 출력
    cout << "Result Matrix:" << endl;
    print_matrix(C);

    // 결과를 파일에 저장
    ofstream result_file("result.txt", ios::app);
    if (result_file.is_open()) {
        result_file << "Result Matrix (" << m << "x" << n << "):\n";
        for (const auto& row : C) {
            for (int val : row) {
                result_file << setw(4) << val << " ";
            }
            result_file << endl;
        }
        result_file << "\n";
        result_file.close();
    }
    else {
        cout << "Unable to open file for writing result.\n";
    }

    // 처리 시간 계산
    chrono::duration<double> duration = end_time - start_time;
    cout << "Processing Time: " << fixed << setprecision(3) << duration.count() << " sec\n\n";

    return 0;
}
