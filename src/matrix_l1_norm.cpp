#include <iostream>
#include <vector>
#include <cstdint>
#include <chrono>
#include <algorithm>
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

double benchmark(uint64_t (*func)(const std::vector<uint64_t>&, size_t),
                 const std::vector<uint64_t>& mat, size_t N,
                 int outer_reps = 10, int inner_reps = 20) {
    std::vector<double> times;
    volatile uint64_t sink = 0;  // защита от оптимизации

    for (int i = 0; i < outer_reps; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < inner_reps; ++j) {
            sink += func(mat, N);
        }
        auto end = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(end - start).count();
        times.push_back(elapsed / inner_reps);
    }

    if (sink == 0) std::cerr << "";
    return *std::min_element(times.begin(), times.end());
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <row|col|nop> [N]\n";
        return 1;
    }
    std::string method = argv[1];
    size_t N = (argc > 2) ? std::stoull(argv[2]) : 4096;
    
    std::vector<uint64_t> matrix(N * N);
    for (size_t i = 0; i < N * N; ++i)
        matrix[i] = i % 100;
    
    double time = 0.0;
    if (method == "row")
        time = benchmark(l1_norm_row_major, matrix, N);
    else if (method == "col")
        time = benchmark(l1_norm_column_major, matrix, N);
    else if (method == "nop") {
        // Для NOOP просто замеряем пустой цикл
        volatile uint64_t sink = 0;
        std::vector<double> times;
        for (int i = 0; i < 10; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < 20; ++j) { sink += 0; }
            auto end = std::chrono::high_resolution_clock::now();
            times.push_back(std::chrono::duration<double>(end - start).count() / 20);
        }
        time = *std::min_element(times.begin(), times.end());
    } else {
        std::cerr << "Invalid method\n";
        return 1;
    }
    
    std::cout << "Method: " << method << "\n";
    std::cout << "N = " << N << "\n";
    std::cout << "Time (min of 10) = " << time << " s\n";
    return 0;
}