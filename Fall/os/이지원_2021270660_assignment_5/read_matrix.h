#ifndef READ_MATRIX_H
#define READ_MATRIX_H

#include <vector>
#include <string>

#define MAX_ROW 100
#define MAX_COL 100

void read_matrix(std::vector<std::vector<int>>& A, int& Ar, int& Ac, const char* filename);

#endif // READ_MATRIX_H
