#!/bin/bash
echo "Row-major:"
for i in {1..5}; do
    perf stat -e L1-dcache-load-misses,instructions,cycles ./matrix_l1_norm row 4096 2>&1 | grep -E "L1-dcache|instructions|cycles|seconds"
    echo "---"
done

echo "Column-major:"
for i in {1..5}; do
    perf stat -e L1-dcache-load-misses,instructions,cycles ./matrix_l1_norm col 4096 2>&1 | grep -E "L1-dcache|instructions|cycles|seconds"
    echo "---"
done

echo "NOOP:"
perf stat -e L1-dcache-load-misses,instructions,cycles ./matrix_l1_norm nop 4096 2>&1 | grep -E "L1-dcache|instructions|cycles|seconds"
echo "---"