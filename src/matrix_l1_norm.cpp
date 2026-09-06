#include <iostream>
#include <vector>
#include <cstdint>
#include <chrono>
#include <cstdlib>

uint64_t l1_norm_column_major(const std::vector<uint64_t>& mat, size_t N) {
    uint64_t sum = 0;
    for (size_t col = 0; col < N; ++col) {
        for (size_t row = 0; row < N; ++row) {
            sum += mat[row * N + col];
        }
    }
    return sum;
}

uint64_t l1_norm_row_major(const std::vector<uint64_t>& mat, size_t N) {
    uint64_t sum = 0;
    for (size_t row = 0; row < N; ++row) {
        for (size_t col = 0; col < N; ++col) {
            sum += mat[row * N + col];
        }
    }
    return sum;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <row|col> [N]\n";
        return 1;
    }
    std::string method = argv[1];
    size_t N = (argc > 2) ? std::stoull(argv[2]) : 4096;
    
    // Выделяем память и заполняем матрицу (row-major)
    std::vector<uint64_t> matrix(N * N);
    for (size_t i = 0; i < N * N; ++i)
        matrix[i] = (i % 100); // значения не важны

    auto start = std::chrono::high_resolution_clock::now();
    uint64_t result = 0;
    if (method == "row")
        result = l1_norm_row_major(matrix, N);
    else if (method == "col")
        result = l1_norm_column_major(matrix, N);
    else {
        std::cerr << "NOOP\n";
    }
    auto end = std::chrono::high_resolution_clock::now();
    double time = std::chrono::duration<double>(end - start).count();

    std::cout << "Method: " << method << "\n";
    std::cout << "N = " << N << "\n";
    std::cout << "Result = " << result << "\n";
    std::cout << "Time = " << time << " s\n";
    return 0;
}