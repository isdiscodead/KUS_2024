#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include "read_matrix.h"

using namespace std;

// ��� ���� �Լ� (��Ƽ������)
vector<vector<int>> matrix_multiply(const vector<vector<int>>& A, const vector<vector<int>>& B, int m, int k, int n) {
    vector<vector<int>> C(m, vector<int>(n, 0));
    vector<thread> threads;

    // �� C[i][j] �׸��� ����ϴ� ������
    auto compute_entry = [&](int i, int j) {
        this_thread::sleep_for(chrono::seconds(1));  // 1�� ���
        for (int l = 0; l < k; ++l) {
            C[i][j] += A[i][l] * B[l][j];
        }
        };

    // ������ ���� (�� C[i][j] �׸� ���)
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            threads.push_back(thread(compute_entry, i, j));  // �� ������ ����
        }
    }

    // ��� ������ ���� ���
    for (auto& th : threads) {
        th.join();  // ������ ���� ���
    }

    return C;
}

// ��� ��� �Լ�
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

    // �ð� ���� ����
    auto start_time = chrono::high_resolution_clock::now();

    // ù ��° ��� ���� �б�
    read_matrix(A, m, k, argv[1]);

    // �� ������ �о�� ��� ������ ����
    for (int arg = 1; arg < argc - 1; ++arg) {
        if (arg > 1) {
            A = C;
        }
        
        // ���� ��� ���� �б�
        read_matrix(B, k, n, argv[arg + 1]);

        cout << "(" << setw(5) << m << "x" << setw(5) << k << ") X ("
            << setw(5) << k << "x" << setw(5) << n << ") = ("
            << setw(5) << m << "x" << setw(5) << n << ")" << endl;

        // ��� ���� ����
        C = matrix_multiply(A, B, m, k, n);
    }

    // �ð� ���� ��
    auto end_time = chrono::high_resolution_clock::now();

    // ��� ��� ���
    cout << "Result Matrix:" << endl;
    print_matrix(C);

    // ����� ���Ͽ� ����
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

    // ó�� �ð� ���
    chrono::duration<double> duration = end_time - start_time;
    cout << "Processing Time: " << fixed << setprecision(3) << duration.count() << " sec\n\n";

    return 0;
}
