#include <cstdint>
#include <cstring>
#include <iostream>
#include <string_view>
#include <benchmark/benchmark.h>
#include <cpy.hpp>

using namespace hpmem;

template <typename Impl>
static void BM_copy(benchmark::State& state) {
    const uint64_t size = state.range(0);
    char* src           = new char[size];
    char* dst           = new char[size];
    __builtin_memset(src, 'A', size);

    for (auto _ : state) {
        Impl::copy(dst, src, size);
        benchmark::DoNotOptimize(dst);
    }

    src[size - 1] = 0;
    dst[size - 1] = 0;
    if (std::string_view(dst) != src) {
        std::cout << "wrong\n";
    }

    delete[] src;
    delete[] dst;
    state.SetBytesProcessed(state.iterations() * size);
}

struct hpmem_hpcpy {
    static void copy(void* dst, void* src, uint64_t length) { hpcpy(dst, src, length); }
};

struct builtin_memcpy {
    static void copy(void* dst, void* src, uint64_t length) { __builtin_memcpy(dst, src, length); }
};

struct std_memcpy {
    static void copy(void* dst, void* src, uint64_t length) { std::memcpy(dst, src, length); }
};

#define BENCH(_)                                                                                   \
    BENCHMARK_TEMPLATE(BM_copy, hpmem_hpcpy)->Arg(_);                                              \
    BENCHMARK_TEMPLATE(BM_copy, builtin_memcpy)->Arg(_);                                           \
    BENCHMARK_TEMPLATE(BM_copy, std_memcpy)->Arg(_)

// BENCH(1 << 0);
// BENCH(1 << 1);
// BENCH(1 << 2);
// BENCH((1 << 2) + (1 << 1));
// BENCH(1 << 3);
// BENCH((1 << 3) + (1 << 1));
// BENCH((1 << 3) + (1 << 2));
// BENCH(1 << 4);
// BENCH((1 << 4) + (1 << 1));
// BENCH((1 << 4) + (1 << 2));
// BENCH((1 << 4) + (1 << 3));
// BENCH(1 << 5);
// BENCH((1 << 5) + (1 << 1));
// BENCH((1 << 5) + (1 << 2));
// BENCH((1 << 5) + (1 << 3));
// BENCH((1 << 5) + (1 << 4));
BENCH(1 << 6);
BENCH((1 << 6) + (1 << 1));
BENCH((1 << 6) + (1 << 2));
BENCH((1 << 6) + (1 << 3));
BENCH((1 << 6) + (1 << 4));
BENCH((1 << 6) + (1 << 5));
BENCH(1 << 7);
BENCH((1 << 7) + (1 << 1));
BENCH((1 << 7) + (1 << 2));
BENCH((1 << 7) + (1 << 3));
BENCH((1 << 7) + (1 << 4));
BENCH((1 << 7) + (1 << 5));
BENCH((1 << 7) + (1 << 6));
BENCH(1 << 8);
BENCH((1 << 8) + (1 << 1));
BENCH((1 << 8) + (1 << 2));
BENCH((1 << 8) + (1 << 3));
BENCH((1 << 8) + (1 << 4));
BENCH((1 << 8) + (1 << 5));
BENCH((1 << 8) + (1 << 6));
BENCH((1 << 8) + (1 << 7));
BENCH(1 << 9);
BENCH((1 << 9) + (1 << 1));
BENCH((1 << 9) + (1 << 2));
BENCH((1 << 9) + (1 << 3));
BENCH((1 << 9) + (1 << 4));
BENCH((1 << 9) + (1 << 5));
BENCH((1 << 9) + (1 << 6));
BENCH((1 << 9) + (1 << 7));
BENCH((1 << 9) + (1 << 8));
BENCH(1 << 10);
BENCH((1 << 10) + (1 << 1));
BENCH((1 << 10) + (1 << 2));
BENCH((1 << 10) + (1 << 3));
BENCH((1 << 10) + (1 << 4));
BENCH((1 << 10) + (1 << 5));
BENCH((1 << 10) + (1 << 6));
BENCH((1 << 10) + (1 << 7));
BENCH((1 << 10) + (1 << 8));
BENCH((1 << 10) + (1 << 9));
BENCH(1 << 11);
BENCH((1 << 11) + (1 << 1));
BENCH((1 << 11) + (1 << 2));
BENCH((1 << 11) + (1 << 3));
BENCH((1 << 11) + (1 << 4));
BENCH((1 << 11) + (1 << 5));
BENCH((1 << 11) + (1 << 6));
BENCH((1 << 11) + (1 << 7));
BENCH((1 << 11) + (1 << 8));
BENCH((1 << 11) + (1 << 9));
BENCH((1 << 11) + (1 << 10));
BENCH(1 << 12);
BENCH((1 << 12) + (1 << 1));
BENCH((1 << 12) + (1 << 2));
BENCH((1 << 12) + (1 << 3));
BENCH((1 << 12) + (1 << 4));
BENCH((1 << 12) + (1 << 5));
BENCH((1 << 12) + (1 << 6));
BENCH((1 << 12) + (1 << 7));
BENCH((1 << 12) + (1 << 8));
BENCH((1 << 12) + (1 << 9));
BENCH((1 << 12) + (1 << 10));
BENCH((1 << 12) + (1 << 11));
BENCH(1 << 13);
BENCH((1 << 13) + (1 << 1));
BENCH((1 << 13) + (1 << 2));
BENCH((1 << 13) + (1 << 3));
BENCH((1 << 13) + (1 << 4));
BENCH((1 << 13) + (1 << 5));
BENCH((1 << 13) + (1 << 6));
BENCH((1 << 13) + (1 << 7));
BENCH((1 << 13) + (1 << 8));
BENCH((1 << 13) + (1 << 9));
BENCH((1 << 13) + (1 << 10));
BENCH((1 << 13) + (1 << 11));
BENCH((1 << 13) + (1 << 12));
BENCH(1 << 14);
BENCH((1 << 14) + (1 << 1));
BENCH((1 << 14) + (1 << 2));
BENCH((1 << 14) + (1 << 3));
BENCH((1 << 14) + (1 << 4));
BENCH((1 << 14) + (1 << 5));
BENCH((1 << 14) + (1 << 6));
BENCH((1 << 14) + (1 << 7));
BENCH((1 << 14) + (1 << 8));
BENCH((1 << 14) + (1 << 9));
BENCH((1 << 14) + (1 << 10));
BENCH((1 << 14) + (1 << 11));
BENCH((1 << 14) + (1 << 12));
BENCH((1 << 14) + (1 << 13));
BENCH(1 << 15);
BENCH((1 << 15) + (1 << 1));
BENCH((1 << 15) + (1 << 2));
BENCH((1 << 15) + (1 << 3));
BENCH((1 << 15) + (1 << 4));
BENCH((1 << 15) + (1 << 5));
BENCH((1 << 15) + (1 << 6));
BENCH((1 << 15) + (1 << 7));
BENCH((1 << 15) + (1 << 8));
BENCH((1 << 15) + (1 << 9));
BENCH((1 << 15) + (1 << 10));
BENCH((1 << 15) + (1 << 11));
BENCH((1 << 15) + (1 << 12));
BENCH((1 << 15) + (1 << 13));
BENCH((1 << 15) + (1 << 14));
BENCH(1 << 16);
BENCH((1 << 16) + (1 << 1));
BENCH((1 << 16) + (1 << 2));
BENCH((1 << 16) + (1 << 3));
BENCH((1 << 16) + (1 << 4));
BENCH((1 << 16) + (1 << 5));
BENCH((1 << 16) + (1 << 6));
BENCH((1 << 16) + (1 << 7));
BENCH((1 << 16) + (1 << 8));
BENCH((1 << 16) + (1 << 9));
BENCH((1 << 16) + (1 << 10));
BENCH((1 << 16) + (1 << 11));
BENCH((1 << 16) + (1 << 12));
BENCH((1 << 16) + (1 << 13));
BENCH((1 << 16) + (1 << 14));
BENCH((1 << 16) + (1 << 15));
BENCH(1 << 17);
BENCH((1 << 17) + (1 << 1));
BENCH((1 << 17) + (1 << 2));
BENCH((1 << 17) + (1 << 3));
BENCH((1 << 17) + (1 << 4));
BENCH((1 << 17) + (1 << 5));
BENCH((1 << 17) + (1 << 6));
BENCH((1 << 17) + (1 << 7));
BENCH((1 << 17) + (1 << 8));
BENCH((1 << 17) + (1 << 9));
BENCH((1 << 17) + (1 << 10));
BENCH((1 << 17) + (1 << 11));
BENCH((1 << 17) + (1 << 12));
BENCH((1 << 17) + (1 << 13));
BENCH((1 << 17) + (1 << 14));
BENCH((1 << 17) + (1 << 15));
BENCH((1 << 17) + (1 << 16));
BENCH(1 << 18);
BENCH((1 << 18) + (1 << 1));
BENCH((1 << 18) + (1 << 2));
BENCH((1 << 18) + (1 << 3));
BENCH((1 << 18) + (1 << 4));
BENCH((1 << 18) + (1 << 5));
BENCH((1 << 18) + (1 << 6));
BENCH((1 << 18) + (1 << 7));
BENCH((1 << 18) + (1 << 8));
BENCH((1 << 18) + (1 << 9));
BENCH((1 << 18) + (1 << 10));
BENCH((1 << 18) + (1 << 11));
BENCH((1 << 18) + (1 << 12));
BENCH((1 << 18) + (1 << 13));
BENCH((1 << 18) + (1 << 14));
BENCH((1 << 18) + (1 << 15));
BENCH((1 << 18) + (1 << 16));
BENCH((1 << 18) + (1 << 17));
BENCH(1 << 19);
BENCH((1 << 19) + (1 << 1));
BENCH((1 << 19) + (1 << 2));
BENCH((1 << 19) + (1 << 3));
BENCH((1 << 19) + (1 << 4));
BENCH((1 << 19) + (1 << 5));
BENCH((1 << 19) + (1 << 6));
BENCH((1 << 19) + (1 << 7));
BENCH((1 << 19) + (1 << 8));
BENCH((1 << 19) + (1 << 9));
BENCH((1 << 19) + (1 << 10));
BENCH((1 << 19) + (1 << 11));
BENCH((1 << 19) + (1 << 12));
BENCH((1 << 19) + (1 << 13));
BENCH((1 << 19) + (1 << 14));
BENCH((1 << 19) + (1 << 15));
BENCH((1 << 19) + (1 << 16));
BENCH((1 << 19) + (1 << 17));
BENCH((1 << 19) + (1 << 18));
BENCH(1 << 20);

BENCHMARK_MAIN();

// int main() {
//     char* src = new char[130];
//     __builtin_memset(src, 'a', 130);
//     src[129]  = 0;
//     char* dst = new char[130];
//     dst[129]  = 0;
//
//     hpcpy(dst, src, 130);
//     if (std::string_view(dst) != src) {
//         std::cout << "wrong!\n";
//     }
//
//     delete[] src;
//     delete[] dst;
//     return 0;
// }
