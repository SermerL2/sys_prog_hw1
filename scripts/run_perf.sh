#!/bin/bash
echo "=== Row-major ==="
perf stat --repeat 10 -e L1-dcache-load-misses,instructions,cycles ./matrix_l1_norm row 4096

echo "=== Column-major ==="
perf stat --repeat 10 -e L1-dcache-load-misses,instructions,cycles ./matrix_l1_norm col 4096

echo "=== NOOP ==="
perf stat --repeat 10 -e L1-dcache-load-misses,instructions,cycles ./matrix_l1_norm nop 4096